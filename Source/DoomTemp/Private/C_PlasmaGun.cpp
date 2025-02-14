// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PlasmaGun.h"

void UC_PlasmaGun::OnFire()
{
	if (CurrentAmmo <= 0)
		return;

	// 모드를 사용 중일 때
	if (bUsingMode) {

	}
	// 그냥 발사만 할 때
	else {
		Super::OnFire();
	}
}
