// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialWidget.generated.h"

/**
 * 
 */
UCLASS()
class RUN_TO_BRIDGE_API UTutorialWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 가이드 텍스트
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* guideText;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	// 현재 튜토리얼 단계
	int32 CurrentStep;

	// 단계 업데이트 함수
	void UpdateStep(int32 NewStep);
	
	// 입력 감지 플래그
	bool bMoved;
};
