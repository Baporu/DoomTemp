// Fill out your copyright notice in the Description page of Project Settings.


#include "C_ShotGun.h"
#include "C_GunBullet.h"
#include "C_Pellet.h"
#include "C_PlayerCharacter.h"
#include "C_PlayerAnimInstance.h"

void UC_ShotGun::BeginPlay()
{
	Super::BeginPlay();

	// Limit SpreadPercent and Multiplier
	if (SpreadPercent > 1.0f)
		SpreadPercent = 1.0f;
	if (FireRateMultiplier > 1.0f)
		FireRateMultiplier = 1.0f;
}

void UC_ShotGun::OnFire()
{
	if (CurrentAmmo <= 0)
		return;
	me->Anim->PlayShootAnim();
	
	FTransform firePos = GetSocketTransform(TEXT("FirePosition"));
	// ������ ũ�⿡ ���� Socket�� Transform�� �޶����� ��찡 �ִµ�,
	// �׷� ��� �Ѿ��� ũ�Ⱑ ���� ����Ǵ� ���� �����ϱ� ���� ũ�⸦ 1.0���� �����Ѵ�.
	firePos.SetScale3D(FVector(1.0));
	for (int32 i = 0; i < PelletCount; i++) {
		AC_GunBullet* bullet = GetWorld()->SpawnActor<AC_GunBullet>(BulletFactory, firePos);
		bullet->OnBulletInit(BulletDamage);

		AC_Pellet* pellet = Cast<AC_Pellet>(bullet);
		pellet->ApplySpread(SpreadPercent);
	}

	--CurrentAmmo;
}

void UC_ShotGun::OnUseMode()
{
	bUsingMode = !bUsingMode;

	// When Using Mode, Fire Rate and Bullet Spread are Improved.
	if (bUsingMode) {
		FireRate *= FireRateMultiplier;
		SpreadPercent /= 2;
	}
	else {
		FireRate /= FireRateMultiplier;
		SpreadPercent *= 2;
	}

	AC_PlayerCharacter* player = Cast<AC_PlayerCharacter>(GetOwner());
	if (player) {
		player->SetFireRate(FireRate);
	}
}
