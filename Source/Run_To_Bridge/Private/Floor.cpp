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
	
	// 박스 콜라이더 크기 설정 (두께를 20(10*2)으로 얇게 설정)
	FVector boxSize = FVector(150.0f, 150.f, 10.f);
	boxComp->SetBoxExtent(boxSize);

	// 메쉬 컴포넌트 설정
	meshComp->SetRelativeScale3D(FVector(3.f, 3.f, 0.5f));
	
	// 메쉬의 윗면과 콜리전의 윗면을 일치시키기 위해 메쉬를 아래로 15유닛 내림
	// (메쉬 높이 50의 절반 25, 콜리전 높이 20의 절반 10 -> 차이 15)
	meshComp->SetRelativeLocation(FVector(0.f, 0.f, -15.f));
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
