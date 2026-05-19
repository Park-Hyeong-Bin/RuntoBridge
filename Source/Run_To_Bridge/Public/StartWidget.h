// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartWidget.generated.h"

/**
 * 
 */
UCLASS()
class RUN_TO_BRIDGE_API UStartWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* buttonStart;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* buttonStartTutorial;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* buttonQuit;
	
protected:
	virtual void NativeConstruct() override;

private:
	// 게임 시작 기능
	UFUNCTION()
	void StartGame();
	
	// 튜토리얼 시작 기능
	UFUNCTION()
	void StartTutorial();
	
	// 종료 기능
	UFUNCTION()
	void Quit();
};



