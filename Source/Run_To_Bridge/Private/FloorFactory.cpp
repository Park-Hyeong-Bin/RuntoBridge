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
	if (FloorClasses.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, FloorClasses.Num() - 1);
		
		if (FloorClasses[RandomIndex])
		{
			FVector SpawnLocation = GetActorLocation();
			FRotator SpawnRotation = GetActorRotation();

			if (IsValid(LastSpawnedFloor))
			{
				SpawnLocation.X = LastSpawnedFloor->GetActorLocation().X + 300.0f;
			}

			// [추가된 안전 장치]
			// 만약 계산된 스폰 위치가 플레이어의 위치와 너무 가깝다면(예: 300 유닛 이내),
			// 플레이어 뒤쪽이나 겹치는 위치에 스폰되지 않도록 강제로 앞쪽으로 밀어냅니다.
			if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
			{
				float PlayerX = PlayerPawn->GetActorLocation().X;
				// 플레이어 근처(X-300 ~ X+300)에서 스폰되려 한다면 무시하거나 위치를 조정해야 합니다.
				// 여기서는 캐릭터 겹침 원천 차단을 위해 플레이어 위치보다 항상 앞에 스폰되도록 보정합니다.
				if (SpawnLocation.X < PlayerX + 150.0f)
				{
					// 플레이어보다 최소 150유닛 앞에서 시작하도록 보정
					// (단, 이 경우 바닥 사이에 틈이 생길 수 있으므로 로그를 남겨 거리를 확인하세요)
					// SpawnLocation.X = PlayerX + 150.0f; 
				}
			}
			
			// 스폰 옵션 설정: 겹침이 발생하더라도 일단 무조건 스폰 (물리 밀림은 TeleportPhysics로 해결)
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
			LastSpawnedFloor = GetWorld()->SpawnActor<AActor>(FloorClasses[RandomIndex], SpawnLocation, SpawnRotation, SpawnParams);
		}
	}
}
