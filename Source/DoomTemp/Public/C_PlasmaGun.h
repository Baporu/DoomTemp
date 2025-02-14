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
	// Mouse Left Click Event
	virtual void OnFire() override;
};
