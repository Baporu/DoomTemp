// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PlasmaGun.h"

void UC_PlasmaGun::OnFire()
{
	if (CurrentAmmo <= 0)
		return;

	// When Firing While Use Mode
	if (bUsingMode) {

	}
	// When Firing Without Using Mode
	else {
		Super::OnFire();
	}
}
