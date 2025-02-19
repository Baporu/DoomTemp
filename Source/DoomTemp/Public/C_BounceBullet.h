// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_GunBullet.h"
#include "C_BounceBullet.generated.h"

/**
 * 
 */
UCLASS()
class DOOMTEMP_API AC_BounceBullet : public AC_GunBullet
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AC_BounceBullet();

public:
	// Collision Component
	UPROPERTY(VisibleAnywhere, Category = Basics)
	class USphereComponent* CollisionComp;
	// Mesh Component
	UPROPERTY(VisibleAnywhere, Category = Basics)
	class UStaticMeshComponent* MeshComp;

	// Movement Component
	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* MovementComp;
};
