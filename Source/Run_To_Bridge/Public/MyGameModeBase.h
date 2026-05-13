// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class RUN_TO_BRIDGE_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyGameModeBase();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	// 현재 게임 속도 배율
	UPROPERTY(BlueprintReadWrite, Category = "GameSpeed")
	float CurrentSpeed;

	// 20초마다 증가할 속도 양
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSpeed")
	float SpeedIncreaseAmount;

	// 속도가 증가하는 시간 간격 (20초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSpeed")
	float SpeedIncreaseInterval;

	// 최대 속도 제한
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameSpeed")
	float MaxSpeed;

private:
	// 마지막으로 속도가 증가한 이후 경과된 시간
	float TimerCount;
};
