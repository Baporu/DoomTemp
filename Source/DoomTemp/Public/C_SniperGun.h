// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_GunSkeletalMeshComponent.h"
#include "C_SniperGun.generated.h"

/**
 * 
 */
UCLASS()
class DOOMTEMP_API UC_SniperGun : public UC_GunSkeletalMeshComponent
{
	GENERATED_BODY()
	

private:
	class UCameraComponent* FPSCam;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Sniper UI
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> SniperUIFactory;
	// Sniper UI Widget Instance
	UPROPERTY()
	class UUserWidget* SniperUI;
	// Sniper FX
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	class UParticleSystem* SniperEffect;
	
	UPROPERTY(EditDefaultsOnly)
	int32 SnipeDamage = 40;


	// Mouse Left Click Event
	virtual void OnFire() override;
	// Mouse Right Click Event
	virtual void OnUseMode() override;

};
