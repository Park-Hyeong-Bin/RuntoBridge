// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/BoxComponent.h"


// Sets default values
AItem::AItem()
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
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

