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

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputMappingContext* imc_TPS;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* ia_LookUp;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* ia_Turn;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* ia_Move;

	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float walkSpeed = 600.0f;
	
	FVector direction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* ia_Jump;


	void Turn(const struct FInputActionValue& inputValue);

	void LookUp(const struct FInputActionValue& inputValue);

	void Move(const struct FInputActionValue& inputValue);

	void InputJump(const struct FInputActionValue& inputValue);

	void PlayerMove();

};
