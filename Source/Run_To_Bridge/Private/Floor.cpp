// Fill out your copyright notice in the Description page of Project Settings.


#include "Floor.h"
#include "Components/BoxComponent.h"


// Sets default values
AFloor::AFloor()
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
	
	// 박스 콜라이더 크기를 300x300x50 설정 (Extent는 절반인 150x150x25)
	FVector boxSize = FVector(150.f, 150.f, 25.f);
	boxComp->SetBoxExtent(boxSize);

	// 메쉬 컴포넌트 크기를 박스 콜라이더와 맞춤 (기본 큐브 100x100x100 기준)
	meshComp->SetRelativeScale3D(FVector(3.f, 3.f, 0.5f));
	}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//매프레임 x 축 마이너스 방향으로 플레이어 반대방향으로 이동
	FVector NewLocation = GetActorLocation();
	NewLocation.X -= MoveSpeed * DeltaTime;
	//바닥 위치 업데이트너가 
	SetActorLocation(NewLocation);
	
	// 2. 만약 지정된 파괴 위치보다 더 뒤로 갔다면 메모리 관리를 위해 제거
	if (NewLocation.X <= DestroyLocationX){
		Destroy();
	}
}

