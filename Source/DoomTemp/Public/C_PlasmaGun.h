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
	

public:
	UC_PlasmaGun();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
//	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	class UParticleSystem* LaserEffect;
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	class UParticleSystem* LaserHitEffect;
	
	class UParticleSystemComponent* LaserComp;
	class UParticleSystemComponent* LaserHitComp;

	UPROPERTY(EditAnywhere, Category = Laser)
	int32 LaserDamage = 10;
	UPROPERTY(EditAnywhere, Category = Laser)
	float LaserTime = 0.5f;
	float LaserTimer;
	bool bUseLaser = false;

	class USoundBase* LaserSound;


	// Mouse Left Click Event
	virtual void OnFire() override;
	// Mouse Right Click Event
	virtual void OnUseMode() override;

	void ToggleLaser(bool InActive);

};
