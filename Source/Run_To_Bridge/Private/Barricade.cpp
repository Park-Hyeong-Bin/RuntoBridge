// Fill out your copyright notice in the Description page of Project Settings.


#include "Barricade.h"
#include "Components/BoxComponent.h"

// Sets default values
ABarricade::ABarricade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 박스 콜리전 컴포넌트 생성
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));
	
	// 생성한 박스 콜리전 컴포넌트를 최상단 컴포넌트로 설정
	SetRootComponent(boxComp);
	
	// 스태틱메시 컴포넌트 생성
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh Component"));
	
	// 박스 콜리전 자식으로 스태틱 메시 컴포넌트 설정
	meshComp->SetupAttachment(boxComp);
	
	// 박스 콜라이더 크기를 100,100,100설정
	FVector boxSize = FVector(100.f, 100.f, 100.f);
	boxComp->SetBoxExtent(boxSize);

	// 메쉬 컴포넌트 크기를 박스 콜라이더와 맞춤 (기본 큐브 100x100x100 기준)
	meshComp->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
}


// Called when the game starts or when spawned
void ABarricade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABarricade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

