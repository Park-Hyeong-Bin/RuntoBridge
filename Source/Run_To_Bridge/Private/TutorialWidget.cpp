// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UTutorialWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CurrentStep = 0;
	bMoved = false;
	UpdateStep(CurrentStep);
}

void UTutorialWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	if (CurrentStep == 0)
	{
		// A 또는 D 키 입력 감지
		if (PC->IsInputKeyDown(EKeys::A) || PC->IsInputKeyDown(EKeys::D))
		{
			bMoved = true;
		}

		// 키를 뗐을 때 다음 단계로 (이동을 한 번이라도 했다면)
		if (bMoved && !PC->IsInputKeyDown(EKeys::A) && !PC->IsInputKeyDown(EKeys::D))
		{
			UpdateStep(1);
		}
	}
	else if (CurrentStep == 1)
	{
		// F 키 입력 감지 시 메인 메뉴로 이동 후 종료
		if (PC->IsInputKeyDown(EKeys::F))
		{
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("Bridge"));
			RemoveFromParent();
		}
	}
}

void UTutorialWidget::UpdateStep(int32 NewStep)
{
	CurrentStep = NewStep;

	if (!guideText) return;

	switch (CurrentStep)
	{
	case 0:
		guideText->SetText(FText::FromString(TEXT("A와 D를 이용하여 좌우로 움직일 수 있습니다.")));
		break;
	case 1:
		guideText->SetText(FText::FromString(TEXT("F를 눌러 튜토리얼을 종료합니다.")));
		break;
	default:
		break;
	}
}
