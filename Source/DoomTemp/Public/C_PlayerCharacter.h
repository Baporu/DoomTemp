// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_PlayerCharacter.generated.h"

// Define Player's Weapon State
UENUM()
enum class EWeaponType : uint8 {
	Plasma,
	Sniper,
	Shotgun,
	MAX
};

// Define Player's Attack Type
UENUM()
enum class EAttackType {
	Fist,
	Gun,
	GloryKill,
	Chainsaw,
	MAX
};

UCLASS()
class DOOMTEMP_API AC_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AC_PlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	// FPS Camera
	UPROPERTY(EditAnywhere)
	class UCameraComponent* FPSCamComp;

	// FPS Player Mesh
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* FPSMeshComp;

	// Input Mapping Context
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_FPS;

	// Camera Input Action
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_LookUp;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Turn;

	// Player Movement Input Action
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;
	// Movement Direction
	FVector MoveDir;

	// Player Jump Input Action
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Jump;

	// Player Dash Input Action
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Dash;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int32 MaxDashCount = 2;
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	int32 CurDashCount = MaxDashCount;

	FVector DashDir = FVector::ZeroVector;
	float DashSpeed = 5.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float DashCoolTime = 4.0f;
	// Dash Count Reset Timer
	float DashTimer = DashCoolTime;

	// Player Fire Input Action
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Fire;
	// Is Firing or Not
	bool bIsFire = false;

	// Player Weapon State
	UPROPERTY(VisibleAnywhere, Category = "Guns")
	EWeaponType mWeaponType = EWeaponType::Sniper;
	UPROPERTY(EditDefaultsOnly, Category = "Guns")
	class UC_GunSkeletalMeshComponent* PlasmaMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Guns")
	class UC_GunSkeletalMeshComponent* SniperMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Guns")
	class UC_GunSkeletalMeshComponent* ShotgunMesh;

	// Guns Array
// 	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
// 	TSubclassOf<class UC_GunSkeletalMeshComponent> Guns[3];

	// HP
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int32 MaxHP = 100;
	int32 CurrentHP;

	// Melee Attack Damage
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int32 MeleeDamage = 5;

	// Player Mode Input Action
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_UseMode;


	void OnLookUp(const struct FInputActionValue& inputValue);
	void OnTurn(const struct FInputActionValue& inputValue);

	void OnMove(const struct FInputActionValue& inputValue);
	void PlayerMove();

	void OnJump(const struct FInputActionValue& inputValue);

	void OnDash(const struct FInputActionValue& inputValue);
	void ResetDashDir(const struct FInputActionValue& inputValue);
	void ResetDashCount(float InDeltaTime);

	void OnFire(const struct FInputActionValue& inputValue);
	void Fire_Plasma();
	void Fire_Sniper();
	void Fire_Shotgun();

	void OnUseMode(const struct FInputActionValue& inputValue);

	void ChangeWeapon(EWeaponType InChangeType);
	void SetWeaponActive(EWeaponType InChangeType, bool InActive);

	UCameraComponent* GetCameraComponent();

};
