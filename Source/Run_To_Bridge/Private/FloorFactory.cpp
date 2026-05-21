// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorFactory.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AFloorFactory::AFloorFactory()
{
	PrimaryActorTick.bCanEverTick = true;
	LastSpawnedFloor = nullptr;
}

void AFloorFactory::BeginPlay()
{
	Super::BeginPlay();
	SpawnFloor();
}

void AFloorFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(LastSpawnedFloor))
	{
		float FloorWidth = 300.0f; 
		float Distance = GetActorLocation().X - LastSpawnedFloor->GetActorLocation().X;
		
		while (Distance >= FloorWidth)
		{
			SpawnFloor();
			
			if (IsValid(LastSpawnedFloor))
			{
				Distance = GetActorLocation().X - LastSpawnedFloor->GetActorLocation().X;
			}
			else break;
		}
	}
	else
	{
		SpawnFloor();
	}
}

void AFloorFactory::SpawnFloor()
{
	if (FloorClasses.Num() <= 0) return;

	TSubclassOf<AActor> ClassToSpawn = nullptr;

	// 1. 이미 연속 스폰 중인지 확인
	if (ConsecutiveSpawnCount > 0 && CurrentConsecutiveClass != nullptr)
	{
		ClassToSpawn = CurrentConsecutiveClass;
		ConsecutiveSpawnCount--;
		
		// 연속 스폰이 모두 끝나면 클래스 정보 초기화
		if (ConsecutiveSpawnCount <= 0)
		{
			CurrentConsecutiveClass = nullptr;
		}
	}
	else
	{
		// 2. 랜덤으로 새로운 바닥 선택 (이전 바닥과 겹치지 않도록 반복)
		if (FloorClasses.Num() > 1)
		{
			do
			{
				int32 RandomIndex = FMath::RandRange(0, FloorClasses.Num() - 1);
				ClassToSpawn = FloorClasses[RandomIndex];
			} while (ClassToSpawn == LastSpawnedClass);
		}
		else
		{
			ClassToSpawn = FloorClasses[0];
		}

		// 3. 만약 선택된 클래스가 SpecialConsecutiveClasses 배열에 포함되어 있다면 연속 스폰 카운트 설정
		if (ClassToSpawn && SpecialConsecutiveClasses.Contains(ClassToSpawn))
		{
			CurrentConsecutiveClass = ClassToSpawn;
			// 방금 하나 뽑았으므로, 앞으로 (Max - 1)번 더 스폰하도록 설정
			ConsecutiveSpawnCount = MaxConsecutiveSpawns - 1; 
		}
	}

	if (ClassToSpawn)
	{
		// 다음에 뽑을 때 참고할 수 있도록 현재 클래스 저장
		LastSpawnedClass = ClassToSpawn;

		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();

		if (IsValid(LastSpawnedFloor))
		{
			SpawnLocation.X = LastSpawnedFloor->GetActorLocation().X + 300.0f;
		}

		if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			float PlayerX = PlayerPawn->GetActorLocation().X;
			if (SpawnLocation.X < PlayerX + 150.0f)
			{
				// SpawnLocation.X = PlayerX + 150.0f;
			}
		}
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		LastSpawnedFloor = GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
	}
}
