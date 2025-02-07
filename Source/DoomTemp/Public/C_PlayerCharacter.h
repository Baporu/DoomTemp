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
	Shotgun
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

	// Camera Up Down Input Action
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_LookUp;
	// Camera Left Right Input Action
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
	// Max Dash Count
	UPROPERTY(EditDefaultsOnly)
	int32 MaxDashCount = 2;
	// Current Dash Count
	UPROPERTY(VisibleAnywhere)
	int32 CurDashCount = MaxDashCount;
	// Dash Direction
	FVector DashDir = FVector::ZeroVector;
	// Dash Speed
	float DashSpeed = 5.0f;
	// Dash Cool-time
	UPROPERTY(EditDefaultsOnly)
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
	EWeaponType mWeaponType;
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
	UPROPERTY(EditDefaultsOnly)
	int32 MaxHP = 100;
	int32 CurrentHP;

	// Melee Attack Damage
	UPROPERTY(EditDefaultsOnly)
	int32 MeleeDamage = 5;


	void OnLookUp(const struct FInputActionValue& inputValue);
	void OnTurn(const struct FInputActionValue& inputValue);

	void OnMove(const struct FInputActionValue& inputValue);
	void PlayerMove();

	void OnJump(const struct FInputActionValue& inputValue);

	void OnDash(const struct FInputActionValue& inputValue);
	void ResetDashDir(const struct FInputActionValue& inputValue);
	void ResetDashCount(float InDeltaTime);

	void OnFire(const struct FInputActionValue& inputValue);

	void Type_Plasma();
	void Type_Sniper();
	void Type_Shotgun();

	UCameraComponent* GetCameraComponent();

};
