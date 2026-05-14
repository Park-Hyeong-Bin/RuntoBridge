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

	// 커스텀 콜리전 프로필 설정
	boxComp->SetCollisionProfileName(TEXT("Floor"));
	
	// 스태틱메시 컴포넌트 생성
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh Component"));
	
	// 박스 콜리전 자식으로 스태틱 메시 컴포넌트 설정
	meshComp->SetupAttachment(boxComp);
	
	// 박스 콜라이더 크기 설정 (두께를 으로 얇게 설정)
	FVector boxSize = FVector(150.0f, 450.f, 25.f);
	boxComp->SetBoxExtent(boxSize);

	// 메쉬 컴포넌트 크기를 박스 콜라이더와 맞춤 (기본 큐브 100x100x100 기준)
	meshComp->SetRelativeScale3D(FVector(3.0f, 9.0f, 0.5f));

	
	
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
	
	// 매프레임 x 축 마이너스 방향으로 이동
	FVector NewLocation = GetActorLocation();
	NewLocation.X -= MoveSpeed * DeltaTime;

	// TeleportPhysics 옵션을 사용하여 이동 시 물리적 마찰이나 충돌 반작용(밀림)을 최소화합니다.
	// 이 옵션은 물리 엔진이 액터의 '속도'를 계산하여 위에 있는 물체를 튕겨내는 것을 방지합니다.
	SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
	
	// 2. 만약 지정된 파괴 위치보다 더 뒤로 갔다면 메모리 관리를 위해 제거
	if (NewLocation.X <= DestroyLocationX){
		Destroy();
	}
}
