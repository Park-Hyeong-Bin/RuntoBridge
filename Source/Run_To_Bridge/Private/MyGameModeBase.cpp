// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"

AMyGameModeBase::AMyGameModeBase()
{
	// Tick 활성화
	PrimaryActorTick.bCanEverTick = true;

	// 초기값 설정
	CurrentSpeed = 1.0f;
	SpeedIncreaseAmount = 0.1f;    // 10%씩 증가
	SpeedIncreaseInterval = 20.0f; // 20초마다
	MaxSpeed = 3.0f;               // 최대 3배속
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

	// 캐릭터 위치 고정 및 로깅
	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		FVector CurrentLoc = PlayerPawn->GetActorLocation();
		
		// X 좌표를 시작 시점의 위치로 강제 고정 (Y, Z는 자유롭게 이동 가능)
		// TeleportPhysics 옵션을 사용하여 물리 엔진과의 충돌 없이 위치를 재설정합니다.
		FVector NewLoc = FVector(FixedPlayerX, CurrentLoc.Y, CurrentLoc.Z);
		PlayerPawn->SetActorLocation(NewLoc, false, nullptr, ETeleportType::TeleportPhysics);

		// 화면에 현재 좌표 표시 (디버깅용)
		if (GEngine)
		{
			FString LocString = FString::Printf(TEXT("Player Fixed X: %.2f | Current Y: %.2f, Z: %.2f"), FixedPlayerX, CurrentLoc.Y, CurrentLoc.Z);
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
