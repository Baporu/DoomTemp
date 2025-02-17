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


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	class UCameraComponent* FPSCam;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxAmmo = 100;
	UPROPERTY(VisibleAnywhere)
	int32 CurrentAmmo;

	UPROPERTY(EditDefaultsOnly)
	int32 BulletDamage = 0;
	UPROPERTY(EditDefaultsOnly)
	float FireRate = 1.0f;

public:
	// Bullet Blueprint
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class AC_GunBullet> BulletFactory;

	// Using Mode or Not
	bool bUsingMode = false;

	// Cross-hair UI Widget
	UPROPERTY(EditAnywhere, Category = GunUI)
	TSubclassOf<class UUserWidget> CrossHairUIFactory;
	// Cross-hair UI Widget Instance
	class UUserWidget* CrossHairUI;


	// Call by Player
	// Mouse Left Click Event
	virtual void OnFire();
	// Mouse Right Click Event
	virtual void OnStartMode();
	virtual void OnUseMode();
	virtual void OnEndMode();

	void IncreaseAmmo();
	void OnGunChanged();

	float GetFireRate();
};
