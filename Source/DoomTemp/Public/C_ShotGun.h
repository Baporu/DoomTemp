// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_GunSkeletalMeshComponent.h"
#include "C_ShotGun.generated.h"

/**
 * 
 */
UCLASS()
class DOOMTEMP_API UC_ShotGun : public UC_GunSkeletalMeshComponent
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	int32 PelletCount = 16;
	// Can't be Over 1.0
	UPROPERTY(EditDefaultsOnly)
	float SpreadPercent = 0.5f;
	// Can't be Over 1.0
	UPROPERTY(EditDefaultsOnly)
	float FireRateMultiplier = 0.5f;

public:



	// Mouse Left Click Event
	virtual void OnFire();
	// Mouse Right Click Event
	virtual void OnUseMode() override;
};
