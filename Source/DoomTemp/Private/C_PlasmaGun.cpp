// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PlasmaGun.h"

void UC_PlasmaGun::OnFire()
{
	if (CurrentAmmo <= 0)
		return;

	// ��带 ��� ���� ��
	if (bUsingMode) {

	}
	// �׳� �߻縸 �� ��
	else {
		Super::OnFire();
	}
}
