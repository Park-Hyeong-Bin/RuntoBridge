// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MenuWidget.h"
#include "StartWidget.h"
#include "TutorialWidget.h"
#include "Components/TextBlock.h"

AMyGameModeBase::AMyGameModeBase()
{
	// Tick 활성화
	PrimaryActorTick.bCanEverTick = true;

	// 초기값 설정
	Score = 0;
	BestScore = 0;
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

	// 현재 레벨 이름 확인
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	// 메인 메뉴(Bridge) 맵인 경우
	if (CurrentLevelName.Equals(TEXT("Bridge"), ESearchCase::IgnoreCase))
	{
		if (startWidgetClass)
		{
			UStartWidget* startUI = CreateWidget<UStartWidget>(GetWorld(), startWidgetClass);
			if (startUI)
			{
				startUI->AddToViewport();
			}
		}
		// 메뉴 맵에서는 게임 로직 관련 초기화(커서 숨김, 플레이어 좌표 저장 등)를 하지 않음
		return;
	}

	// 튜토리얼 맵인 경우
	if (CurrentLevelName.Equals(TEXT("Bridge_Tutorial"), ESearchCase::IgnoreCase))
	{
		// 입력 모드를 게임과 UI 모두 가능하도록 설정 (이동 + 위젯 상호작용)
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			PC->bShowMouseCursor = false;
			FInputModeGameAndUI InputMode;
			PC->SetInputMode(InputMode);
		}

		if (tutorialWidgetClass)
		{
			UTutorialWidget* tutorialUI = CreateWidget<UTutorialWidget>(GetWorld(), tutorialWidgetClass);
			if (tutorialUI)
			{
				tutorialUI->AddToViewport();
			}
		}
		
		// 튜토리얼에서도 X축 고정을 위해 초기 위치 저장
		if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			FixedPlayerX = PlayerPawn->GetActorLocation().X;
		}
		return;
	}

	// 실제 게임 맵인 경우: 입력 모드를 게임 전용으로 초기화하고 마우스 커서 숨김
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
			
			// 최고 기록 로드 및 UI 초기화
			LoadBestScore();
			UpdateScoreUI();
		}
	}
}

void AMyGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	// 메인 메뉴(Bridge) 맵인 경우 게임 로직을 실행하지 않음
	if (CurrentLevelName.Equals(TEXT("Bridge"), ESearchCase::IgnoreCase)) return;

	if (bIsGameOver) return;

	// 캐릭터 위치 관리 (실제 게임 및 튜토리얼 모두 적용)
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
			float RecoverySpeed = 200.0f; // 초당 복귀 속도
			float NewX = FMath::Min(FixedPlayerX, CurrentLoc.X + RecoverySpeed * DeltaTime);
			FVector NewLoc = FVector(NewX, CurrentLoc.Y, CurrentLoc.Z);
			PlayerPawn->SetActorLocation(NewLoc, false, nullptr, ETeleportType::TeleportPhysics);
		}
	}
	else if (!CurrentLevelName.Equals(TEXT("Bridge_Tutorial"), ESearchCase::IgnoreCase))
	{
		// 플레이어가 파괴되었다면 게임 오버 처리 (튜토리얼이 아닐 때만)
		EndGame();
		return;
	}

	// 튜토리얼 맵인 경우 가속 및 점수 로직만 스킵
	if (CurrentLevelName.Equals(TEXT("Bridge_Tutorial"), ESearchCase::IgnoreCase)) return;

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

	// 최고 기록 실시간 업데이트
	if (Score > BestScore)
	{
		BestScore = Score;
	}
	
	UpdateScoreUI();
}

void AMyGameModeBase::UpdateScoreUI()
{
	if (mainUI)
	{
		if (mainUI->scoreData)
		{
			mainUI->scoreData->SetText(FText::AsNumber(Score));
		}

		if (mainUI->bestScoreData)
		{
			mainUI->bestScoreData->SetText(FText::AsNumber(BestScore));
		}

		// 라벨 초기화 (필요시)
		if (mainUI->bestScoreText)
		{
			mainUI->bestScoreText->SetText(FText::FromString(TEXT("내 최고 기록")));
		}
	}
}

void AMyGameModeBase::EndGame()
{
	if (bIsGameOver) return;

	bIsGameOver = true;

	// 최고 기록 저장
	SaveBestScore();

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

void AMyGameModeBase::LoadBestScore()
{
	// Game.ini 파일의 [Score] 섹션에서 BestScore 값을 읽어옵니다.
	if (!GConfig->GetInt(TEXT("Score"), TEXT("BestScore"), BestScore, GGameIni))
	{
		BestScore = 0;
	}
}

void AMyGameModeBase::SaveBestScore()
{
	// Game.ini 파일의 [Score] 섹션에 BestScore 값을 기록합니다.
	GConfig->SetInt(TEXT("Score"), TEXT("BestScore"), BestScore, GGameIni);
	
	// 변경사항을 즉시 파일에 저장(Flush)합니다.
	GConfig->Flush(false, GGameIni);
}
