// Fill out your copyright notice in the Description page of Project Settings.


#include "Trap.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "NiagaraComponent.h"


// Sets default values
ATrap::ATrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 박스 콜리전 컴포넌트 생성
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));
	
	// 생성한 박스 콜리전 컴포넌트를 최상단 컴포넌트로 설정
	SetRootComponent(boxComp);
	
	// 박스 충돌 설정
	boxComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	
	// 스태틱메시 컴포넌트 생성
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh Component"));
	
	// 박스 콜리전 자식으로 스태틱 메시 컴포넌트 설정
	meshComp->SetupAttachment(boxComp);
	
	// 메시 컴포넌트의 충돌은 비활성화 (충돌은 boxComp가 담당)
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 나이아가라 컴포넌트 생성
	fireComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Fire Niagara Component"));
	
	// 박스 콜리전 자식으로 나이아가라 컴포넌트 설정
	fireComponent->SetupAttachment(boxComp);
}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();
	
	// 오버랩 이벤트 바인딩
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ATrap::OnOverlapBegin);

	// fireEffect가 설정되어 있다면 컴포넌트에 할당
	if (fireEffect && fireComponent)
	{
		fireComponent->SetAsset(fireEffect);
		// 항상 재생되도록 활성화
		fireComponent->Activate();
	}
}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATrap::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 플레이어와 오버랩되었는지 확인
	if (OtherActor && OtherActor != this && OtherActor->IsA(APawn::StaticClass()))
	{
		// 플레이어 파괴
		OtherActor->Destroy();
		
		// 트랩도 파괴 (선택 사항, 보통 1회용이면 파괴)
		// Destroy();
	}
}

