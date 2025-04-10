// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "C_PlayerCharacter.h"
#include "Camera/CameraComponent.h"
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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxAmmo = 100;
	UPROPERTY(EditInstanceOnly)
	int32 CurrentAmmo;

	UPROPERTY(EditDefaultsOnly)
	int32 BulletDamage = 0;
	UPROPERTY(EditDefaultsOnly)
	float FireRate = 1.0f;

public:
	UPROPERTY()
	AC_PlayerCharacter* me;
	UPROPERTY()
	UCameraComponent* FPSCam;

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

	// Fire Sound
	class USoundBase* BulletSound;


	// Call by Player
	// Mouse Left Click Event
	virtual void OnFire();
	// Mouse Right Click Event
	virtual void OnStartMode();
	virtual void OnUseMode();
	virtual void OnEndMode();

	void IncreaseAmmo(int32 amount);
	void OnGunChanged();

	float GetFireRate();

	UFUNCTION(BlueprintPure)
	int32 GetMaxAmmo() {return MaxAmmo;};
	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmo() {return CurrentAmmo;};

};
