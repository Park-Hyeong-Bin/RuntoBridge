// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"

AMyGameModeBase::AMyGameModeBase()
{
	// Tick 활성화
	PrimaryActorTick.bCanEverTick = true;

	// 초기값 설정
	CurrentSpeed = 1.5f;           // 시작 속도 1.5배속
	SpeedIncreaseAmount = 0.1f;    // 10%씩 증가
	SpeedIncreaseInterval = 20.0f; // 20초마다
	MaxSpeed = 4.0f;               // 최대 4배속
	TimerCount = 0.0f;
}

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	// 게임 시작 시 초기 속도 적용
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), CurrentSpeed);

	// 게임 시작 시점의 플레이어 X 좌표를 저장하여 고정값으로 사용
	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		FixedPlayerX = PlayerPawn->GetActorLocation().X;
	}
}

void AMyGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 캐릭터 위치 관리
	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
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
			float RecoverySpeed = 400.0f; // 초당 복귀 속도 (필요에 따라 조절 가능)
			float NewX = FMath::Min(FixedPlayerX, CurrentLoc.X + RecoverySpeed * DeltaTime);
			FVector NewLoc = FVector(NewX, CurrentLoc.Y, CurrentLoc.Z);
			PlayerPawn->SetActorLocation(NewLoc, false, nullptr, ETeleportType::TeleportPhysics);
		}

		// 화면에 현재 좌표 표시 (디버깅용)
		if (GEngine)
		{
			FString LocString = FString::Printf(TEXT("Player Fixed X: %.2f | Current X: %.2f, Y: %.2f"), FixedPlayerX, CurrentLoc.X, CurrentLoc.Y);
			GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Cyan, LocString);
		}
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

		// 로그 출력 (디버깅용)
		UE_LOG(LogTemp, Warning, TEXT("Game Speed Increased! Current Speed: %f"), CurrentSpeed);
	}
}
