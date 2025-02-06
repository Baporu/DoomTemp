// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_PlayerCharacter.generated.h"

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

	// Guns Array
	UPROPERTY(EditAnywhere, Category = "Weapons")
	TSubclassOf<class UC_GunSkeletalMeshComponent> Guns[3];
	class UC_GunSkeletalMeshComponent* _Guns[3];


	void LookUp(const struct FInputActionValue& inputValue);
	void Turn(const struct FInputActionValue& inputValue);

	void Move(const struct FInputActionValue& inputValue);
	void PlayerMove();

	void InputJump(const struct FInputActionValue& inputValue);

	void Dash(const struct FInputActionValue& inputValue);
	void ResetDashDir(const struct FInputActionValue& inputValue);
	void ResetDashCount(float InDeltaTime);

	void InputFire(const struct FInputActionValue& inputValue);

	
};
