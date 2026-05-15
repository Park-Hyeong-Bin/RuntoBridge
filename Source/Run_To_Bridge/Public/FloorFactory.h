// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorFactory.generated.h"

UCLASS()
class RUN_TO_BRIDGE_API AFloorFactory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorFactory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 스폰할 바닥 클래스들을 담을 배열
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class AActor>> FloorClasses;

	// 스폰 간격 (초)
	UPROPERTY(EditAnywhere)
	float SpawnInterval = 0.5f;

	// 바닥을 생성하는 함수
	void SpawnFloor();

	// 연속 스폰할 클래스들 (여기에 등록된 클래스가 뽑히면 연속 스폰 시작)
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
	TArray<TSubclassOf<AActor>> SpecialConsecutiveClasses;

	// 연속 스폰할 횟수
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
	int32 MaxConsecutiveSpawns = 5;

private:
	// 타이머 핸들
	FTimerHandle SpawnTimerHandle;

	// 마지막으로 생성된 바닥을 기억합니다.
	UPROPERTY()
	AActor* LastSpawnedFloor;

	// 현재 남은 연속 스폰 횟수
	int32 ConsecutiveSpawnCount = 0;

	// 현재 연속 스폰 중인 클래스
	UPROPERTY()
	TSubclassOf<AActor> CurrentConsecutiveClass = nullptr;
};
