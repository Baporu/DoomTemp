// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "C_PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DOOMTEMP_API UC_PlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFire = false;


	void NativeUpdateAnimation(float DeltaSeconds) override;
};
