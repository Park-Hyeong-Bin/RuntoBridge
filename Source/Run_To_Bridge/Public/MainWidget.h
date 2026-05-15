// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class RUN_TO_BRIDGE_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 점수 텍스트 컴포넌트 - 언리얼에디터 위젯 BP애서 같은 이름으로 바인딩 됨
	// meta 파라미터 옵션 = 정보데이터 옵션으로
	// 변수 노출 시킬 때, 코드와 이름을 다르게하거나, 입력값을 특정 범위로 제한하는 등 사용
	UPROPERTY(EditAnywhere, meta = (BindWidget)) 
	UTextBlock* scoreText;
	
	// 점수 UI 갱신 변수
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* scoreData;
};
