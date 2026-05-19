// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MenuWidget.h"
#include "Components/TextBlock.h"

AMyGameModeBase::AMyGameModeBase()
{
	// Tick 활성화
	PrimaryActorTick.bCanEverTick = true;

	// 초기값 설정
	Score = 0;
	CurrentSpeed = 1.5f;           // 시작 속도 1.5배속
	SpeedIncreaseAmount = 0.1f;    // 10%씩 증가
	SpeedIncreaseInterval = 20.0f; // 20초마다
	MaxSpeed = 4.0f;               // 최대 4배속
	TimerCount = 0.0f;
	TickCounter = 0;
	bIsGameOver = false;
}

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	bIsGameOver = false;

	// 게임 일시정지 해제 (재시작 시 대비)
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	// 입력 모드를 게임 전용으로 초기화하고 마우스 커서 숨김
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}

	// 게임 시작 시 초기 속도 적용
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), CurrentSpeed);

	// 게임 시작 시점의 플레이어 X 좌표를 저장하여 고정값으로 사용
	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		FixedPlayerX = PlayerPawn->GetActorLocation().X;
	}
	
	if (mainWidget)
	{
		mainUI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);
		if (mainUI)
		{
			mainUI->AddToViewport();
			UpdateScoreUI();
		}
	}
}

void AMyGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsGameOver) return;

	// 캐릭터 위치 관리
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		FVector CurrentLoc = PlayerPawn->GetActorLocation();
		
		// 1. 앞으로 너무 나가지 못하게 제한 (고정 위치보다 앞에 있으면 강제 이동)
		if (CurrentLoc.X > FixedPlayerX)
		{
			FVector NewLoc = FVector(FixedPlayerX, CurrentLoc.Y, CurrentLoc.Z);
			PlayerPawn->SetActorLocation(NewLoc, false, nullptr, ETeleportType::TeleportPhysics);
		}
		// 2. 뒤로 밀려난 경우(CurrentLoc.X < FixedPlayerX), 서서히 원래 위치로 복귀
		else if (CurrentLoc.X < FixedPlayerX)
		{
			float RecoverySpeed = 200.0f; // 초당 복귀 속도 (필요에 따라 조절 가능)
			float NewX = FMath::Min(FixedPlayerX, CurrentLoc.X + RecoverySpeed * DeltaTime);
			FVector NewLoc = FVector(NewX, CurrentLoc.Y, CurrentLoc.Z);
			PlayerPawn->SetActorLocation(NewLoc, false, nullptr, ETeleportType::TeleportPhysics);
		}
	}
	else
	{
		// 플레이어가 파괴되었다면 게임 오버 처리
		EndGame();
		return;
	}

	// 타이머 누적 (속도 증가 로직)
	TimerCount += DeltaTime;

	// 20초가 지났고, 아직 최대 속도에 도달하지 않았다면
	if (TimerCount >= SpeedIncreaseInterval && CurrentSpeed < MaxSpeed)
	{
		// 속도 증가
		CurrentSpeed = FMath::Min(CurrentSpeed + SpeedIncreaseAmount, MaxSpeed);
		
		// 전체 게임 속도(Time Dilation)에 적용
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), CurrentSpeed);

		// 타이머 초기화 (남은 시간은 유지하여 오차 방지)
		TimerCount -= SpeedIncreaseInterval;
	}

	// 2틱당 1점 추가 로직
	TickCounter++;
	if (TickCounter >= 2)
	{
		AddScore(1);
		TickCounter = 0;
	}
}

void AMyGameModeBase::AddScore(int32 Amount)
{
	if (bIsGameOver) return;

	Score += Amount;
	
	UpdateScoreUI();
}

void AMyGameModeBase::UpdateScoreUI()
{
	if (mainUI && mainUI->scoreData)
	{
		mainUI->scoreData->SetText(FText::AsNumber(Score));
	}
}

void AMyGameModeBase::EndGame()
{
	if (bIsGameOver) return;

	bIsGameOver = true;

	// 게임 일시정지
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	UE_LOG(LogTemp, Warning, TEXT("Game Over! Final Score: %d"), Score);

	ShowMenu();
}

void AMyGameModeBase::ShowMenu()
{
	if (menuWidgetClass)
	{
		menuUI = CreateWidget<UMenuWidget>(GetWorld(), menuWidgetClass);
		if (menuUI)
		{
			menuUI->AddToViewport();
			
			// 점수 표시
			if (menuUI->score)
			{
				menuUI->score->SetText(FText::AsNumber(Score));
			}

			// 마우스 커서 표시 및 입력 모드 설정
			APlayerController* PC = GetWorld()->GetFirstPlayerController();
			if (PC)
			{
				PC->bShowMouseCursor = true;
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(menuUI->TakeWidget());
				PC->SetInputMode(InputMode);
			}
		}
	}
}
