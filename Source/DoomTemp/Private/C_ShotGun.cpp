// Fill out your copyright notice in the Description page of Project Settings.


#include "C_ShotGun.h"
#include "C_GunBullet.h"

void UC_ShotGun::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(true);
		
}

void UC_ShotGun::OnFire()
{
	if (CurrentAmmo <= 0)
		return;

	// ��带 ��� ���� ��
	if (bUsingMode) {

	}
	// �׳� �߻縸 �� ��
	else {
		FTransform firePos = GetSocketTransform(TEXT("FirePosition"));
		// ������ ũ�⿡ ���� Socket�� Transform�� �޶����� ��찡 �ִµ�,
		// �׷� ��� �Ѿ��� ũ�Ⱑ ���� ����Ǵ� ���� �����ϱ� ���� ũ�⸦ 1.0���� �����Ѵ�.
		firePos.SetScale3D(FVector(1.0));
		for (int32 i = 0; i < PelletCount; i++) {
			FVector randPos = FVector(0.0, FMath::RandRange(-1.0, 1.0), FMath::RandRange(-1.0, 1.0)).GetSafeNormal();
			randPos *= SpreadPercent;

			firePos.SetLocation(FVector(firePos.GetLocation() + randPos));
			GetWorld()->SpawnActor<AC_GunBullet>(BulletFactory, firePos);
		}

		--CurrentAmmo;
	}
}
