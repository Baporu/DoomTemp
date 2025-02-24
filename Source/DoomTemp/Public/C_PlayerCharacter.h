// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_PlayerCharacter.generated.h"

// Define Weapon Type
// Used by Player Weapons
UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	Plasma,
	Sniper,
	Shotgun,
	MAX
};

// Define Player's Attack Type
// Used by Enemy FSM
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
	// Basic Components
	// FPS Camera Component
	UPROPERTY(EditAnywhere, Category = "Basics")
	class UCameraComponent* FPSCamComp;
	// FPS Player Mesh
	UPROPERTY(EditAnywhere, Category = "Basics")
	class USkeletalMeshComponent* FPSMeshComp;
	// Collider to Check Enemy
	UPROPERTY(EditDefaultsOnly, Category = "Basics")
	class UBoxComponent* MeleeComp;
	// Punch Collider
	UPROPERTY(EditDefaultsOnly, Category = "Basics")
	class USphereComponent* PunchComp;

	// Input Mapping Context
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_FPS;

	// Camera
	// Camera Input Actions
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_LookUp;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Turn;

	// Movement
	// Player Movement Input Action
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;
	// Movement Direction
	FVector MoveDir;

	// Jump
	// Player Jump Input Action
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Jump;

	// Dash
	// Player Dash Input Action
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Dash;
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	int32 MaxDashCount = 2;
	UPROPERTY(EditInstanceOnly, Category = "Stats")
	int32 CurDashCount;

	FVector DashDir = FVector::ZeroVector;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float DashDistance = 500.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float DashCoolTime = 2.0f;
	// Dash Count Reset Timer Handle
	FTimerHandle DashTimerHandle;

	// Attack
	// Player Fire Input Action
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Fire;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Guns")
	// Is Firing or Not
	bool bIsFire = false;
	// Shotgun Fire
	bool bShotgun = false;

	// Player Weapon State
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Guns")
	EWeaponType mWeaponType;
	// Weapon Instances
	UPROPERTY(EditDefaultsOnly, Category = "Guns")
	class UC_GunSkeletalMeshComponent* PlasmaMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Guns")
	class UC_GunSkeletalMeshComponent* SniperMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Guns")
	class UC_GunSkeletalMeshComponent* ShotgunMesh;

	// Mode Use Input Action
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_UseMode;
	bool bUseMode = false;

	// Change Weapon Input Action
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_ChangeWeapon;

	// Guns Array
// 	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
// 	TSubclassOf<class UC_GunSkeletalMeshComponent> Guns[3];

	// Stats
	// HP
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int32 MaxHP = 100;
	UPROPERTY(EditInstanceOnly, Category = "Stats")
	int32 CurrentHP;

	// Fire Rate
	UPROPERTY(EditAnywhere, Category = "Stats")
	float FireRate = 1.0f;
	float FireTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Punch;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Saw;
	bool bIsPunching = false;

	// Melee Attack Damage
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int32 MeleeDamage = 5;
	UPROPERTY(VisibleAnywhere, Category = "Melee")
	class AC_Enemy* MeleeTarget;
	UPROPERTY(VisibleAnywhere, Category = "Melee")
	int32 MaxFuel = 3;
	UPROPERTY(VisibleAnywhere, Category = "Melee")
	int32 CurrentFuel = 0;
	// Fuel Regenerate Rate
	float FuelTime = 30.0f;
	// Timer Handle
	FTimerHandle FuelTimerHandle;

	UPROPERTY()
	class UC_PlayerAnimInstance* Anim;


	void OnLookUp(const struct FInputActionValue& inputValue);
	void OnTurn(const struct FInputActionValue& inputValue);

	void OnMove(const struct FInputActionValue& inputValue);
	void PlayerMove();

	void OnJump(const struct FInputActionValue& inputValue);

	void OnDash(const struct FInputActionValue& inputValue);
	void OnDashTime();

	void OnFire(const struct FInputActionValue& inputValue);
	void PlayerFire();
	void Fire_Plasma();
	void Fire_Sniper();
	void Fire_Shotgun();

	void OnUseMode(const struct FInputActionValue& inputValue);

	void OnChangeWeapon(const struct FInputActionValue& inputValue);
	void ChangeWeapon(EWeaponType InChangeType);
	void SetWeaponActive(EWeaponType InChangeType, bool InActive);

	void PlayerHit(int32 InDamage);
	void SetFireRate(float InFireRate);

	void OnPunch(const struct FInputActionValue& inputValue);
	void OnPunchEnd();

	void OnSaw(const struct FInputActionValue& inputValue);

	UCameraComponent* GetCameraComponent();
	UFUNCTION(BlueprintPure)
	UC_GunSkeletalMeshComponent* GetCurrentGun();

	UFUNCTION()
	void OnMeleeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnPunchOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void MeleeDash();

	void OnFuelTime();

	UFUNCTION(BlueprintPure)
	int32 GetPlayerMaxHP() {return MaxHP;};
	UFUNCTION(BlueprintPure)
	int32 GetPlayerCurrentHP() {return CurrentHP;};
	UFUNCTION(BlueprintPure)
	int32 GetPlayerMaxDash() {return MaxDashCount;};
	UFUNCTION(BlueprintPure)
	int32 GetPlayerCurrentDash() {return CurDashCount;};
	UFUNCTION(BlueprintPure)
	int32 GetPlayerMaxFuel() {return MaxFuel;};
	UFUNCTION(BlueprintPure)
	int32 GetPlayerCurrentFuel() {return CurrentFuel;};
	UFUNCTION(BlueprintPure)
	EWeaponType GetPlayerWeaponType() { return mWeaponType; };

};
