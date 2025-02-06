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
	

public:
	// Sets default values for this character's properties
	UC_SniperGun();


	// Call by PlayerCharacter
	//virtual void InputFire();
};
