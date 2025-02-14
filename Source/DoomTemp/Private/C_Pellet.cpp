// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Pellet.h"

void AC_Pellet::BeginPlay()
{
	Super::BeginPlay();

	
}

void AC_Pellet::ApplySpread(float InSpread)
{
	Dir = ( GetActorForwardVector() + ( GetActorRightVector() * FMath::RandRange(-1.0, 1.0) * InSpread ) + ( GetActorUpVector() * FMath::RandRange(-1.0, 1.0) * InSpread ) ).GetSafeNormal();
}
