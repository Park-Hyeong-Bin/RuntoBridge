// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorFactory.h"
#include "Components/BoxComponent.h"

// Sets default values
AFloorFactory::AFloorFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LastSpawnedFloor = nullptr;
}

// Called when the game starts or when spawned
void AFloorFactory::BeginPlay()
{
	Super::BeginPlay();
	
	// SpawnInterval 마다 SpawnFloor 함수를 반복 호출하도록 타이머 설정
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AFloorFactory::SpawnFloor, SpawnInterval, true);
}

// Called every frame
void AFloorFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloorFactory::SpawnFloor()
{
	// 배열에 등록된 클래스가 있는지 확인
	if (FloorClasses.Num() > 0)
	{
		// 0부터 배열 크기 - 1 사이의 랜덤 인덱스 추출
		int32 RandomIndex = FMath::RandRange(0, FloorClasses.Num() - 1);
		
		// 선택된 클래스가 유효한지 확인 후 스폰
		if (FloorClasses[RandomIndex])
		{
			FVector SpawnLocation = GetActorLocation();
			FRotator SpawnRotation = GetActorRotation();

			// 이전 바닥이 있다면, 그 바닥의 현재 위치 바로 뒤에 소환하여 겹침(Overlap)을 방지합니다.
			// 겹침이 발생하면 캐릭터가 물리 엔진에 의해 튕겨나갑니다.
			if (IsValid(LastSpawnedFloor))
			{
				float FloorWidth = 300.0f; // 기본값 (박스 크기 150 * 2)
				if (UBoxComponent* Box = LastSpawnedFloor->FindComponentByClass<UBoxComponent>())
				{
					// 스케일이 반영된 실제 박스 크기를 가져옵니다.
					FloorWidth = Box->GetScaledBoxExtent().X * 2.0f;
				}

				// 이전 바닥의 위치에서 정확히 너비만큼 떨어진 곳에 배치하여 빈틈과 겹침을 모두 제거합니다.
				SpawnLocation.X = LastSpawnedFloor->GetActorLocation().X + FloorWidth;
			}
			
			LastSpawnedFloor = GetWorld()->SpawnActor<AActor>(FloorClasses[RandomIndex], SpawnLocation, SpawnRotation);
		}
	}
}
