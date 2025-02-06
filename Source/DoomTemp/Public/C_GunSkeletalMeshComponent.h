// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "C_GunSkeletalMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class DOOMTEMP_API UC_GunSkeletalMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()
	

public:
	// Sets default values for this character's properties
	UC_GunSkeletalMeshComponent();


	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class AC_GunBullet> BulletFactory;

	// Call by PlayerCharacter
	virtual void InputFire();
};
