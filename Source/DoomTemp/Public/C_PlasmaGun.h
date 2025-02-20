// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_GunSkeletalMeshComponent.h"
#include "C_PlasmaGun.generated.h"

/**
 * 
 */
UCLASS()
class DOOMTEMP_API UC_PlasmaGun : public UC_GunSkeletalMeshComponent
{
	GENERATED_BODY()
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	class UParticleSystem* LaserEffect;
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	class UParticleSystem* LaserHitEffect;
	
	class UParticleSystemComponent* LaserComp;
	class UParticleSystemComponent* LaserHitComp;


	// Mouse Left Click Event
	virtual void OnFire() override;
	// Mouse Right Click Event
	virtual void OnUseMode() override;

	void ToggleLaser(bool InActive);

};
