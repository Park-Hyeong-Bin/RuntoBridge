// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class RUN_TO_BRIDGE_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

	// 박스 충돌체 컴포넌트
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;
	
	// 스태틱 메시 컴포넌트
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
