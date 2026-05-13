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

private:
	// 타이머 핸들
	FTimerHandle SpawnTimerHandle;

	// 마지막으로 생성된 바닥을 기억합니다.
	UPROPERTY()
	AActor* LastSpawnedFloor;
};
