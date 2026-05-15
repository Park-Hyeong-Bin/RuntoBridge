// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyWidget.generated.h"
#include "Blueprint/UserWidget.h"

/**
 * 
 */
UCLASS()
class RUN_TO_BRIDGE_API UMyWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,meta = (BindWidget))
	class UTextBlock* scoreText;
	UPROPERTY(EditAnywhere,	meta = (BindWidget))
	class UTextBlock* scoreData;
};
