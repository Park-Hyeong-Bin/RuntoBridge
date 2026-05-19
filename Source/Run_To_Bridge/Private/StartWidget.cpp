// Fill out your copyright notice in the Description page of Project Settings.


#include "StartWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UStartWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (buttonStart)
	{
		buttonStart->OnClicked.AddDynamic(this, &UStartWidget::StartGame);
	}

	if (buttonStartTutorial)
	{
		buttonStartTutorial->OnClicked.AddDynamic(this, &UStartWidget::StartTutorial);
	}

	if (buttonQuit)
	{
		buttonQuit->OnClicked.AddDynamic(this, &UStartWidget::Quit);
	}

	// 메뉴 위젯이 뜰 때 마우스 커서 표시
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = true;
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());
		PC->SetInputMode(InputMode);
	}
}

void UStartWidget::StartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Stage1"));
}

void UStartWidget::StartTutorial()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("tutorial"));
}

void UStartWidget::Quit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}
