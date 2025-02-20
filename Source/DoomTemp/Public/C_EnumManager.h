// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "C_EnumManager.generated.h"

// Define Player's Gun Type
UENUM()
enum class EGunType : uint8 {
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

/**
 * 
 */
UCLASS()
class DOOMTEMP_API UC_EnumManager : public UObject
{
	GENERATED_BODY()
	
};
