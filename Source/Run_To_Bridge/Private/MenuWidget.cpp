// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 각 버튼 클릭 이벤트에 대응할 함수 연결
	buttonRestart->OnClicked.AddDynamic(this, &UMenuWidget::Restart);
	buttonQuit->OnClicked.AddDynamic(this, &UMenuWidget::Quit);
}

void UMenuWidget::Restart()
{
	// 현재 레벨을 다시 로드(재시작)
	FName LevelName = FName(*UGameplayStatics::GetCurrentLevelName(GetWorld()));
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void UMenuWidget::Quit()
{
	// 현재 실행 중인 월드 종료
	UWorld* currentWorld = GetWorld();
	
	// 앱 종료
	UKismetSystemLibrary::QuitGame(currentWorld, currentWorld->GetFirstPlayerController(),
		EQuitPreference::Quit, false);
}