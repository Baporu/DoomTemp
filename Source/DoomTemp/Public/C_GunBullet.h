// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_GunBullet.generated.h"

UCLASS()
class DOOMTEMP_API AC_GunBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_GunBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	// 충돌체 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class USphereComponent* CollisionComp;
	// 외관 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = BodyMesh)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly)
	int32 Damage = 10;

};
