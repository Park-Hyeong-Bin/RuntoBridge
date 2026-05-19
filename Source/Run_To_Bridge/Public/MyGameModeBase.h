// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainWidget.h"
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

	// 점수 추가 함수
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	// 현재 점수 가져오기
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const { return Score; }

	// 점수 UI 업데이트 함수
	UFUNCTION(BlueprintCallable, Category = "Score")
	void UpdateScoreUI();

	// 게임 오버 처리 함수
	UFUNCTION(BlueprintCallable, Category = "GameLogic")
	void EndGame();

protected:
	virtual void BeginPlay() override;

	// 게임 오버 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameLogic")
	bool bIsGameOver;

	// 현재 점수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Score;

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

	// 위젯 클래스 (에디터에서 할당)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UMainWidget> mainWidget;

	// 메뉴 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UMenuWidget> menuWidgetClass;

	// 생성된 위젯 인스턴스 보관
	UPROPERTY()
	UMainWidget* mainUI;
	
	UPROPERTY()
	class UMenuWidget* menuUI;

	// 메뉴 위젯 표시 함수
	void ShowMenu();
	
private:
	// 마지막으로 속도가 증가한 이후 경과된 시간
	float TimerCount;

	// 고정할 플레이어의 X 좌표
	float FixedPlayerX;

	// 틱 카운터 (점수 계산용)
	int32 TickCounter;
};
