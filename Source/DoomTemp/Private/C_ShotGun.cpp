// Fill out your copyright notice in the Description page of Project Settings.


#include "C_ShotGun.h"
#include "C_GunBullet.h"
#include "C_Pellet.h"
#include "C_PlayerAnimInstance.h"
#include "Kismet/GameplayStatics.h"

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

	// Codes Before Change
	{
		// 	FTransform firePos = GetSocketTransform(TEXT("FirePosition"));
		// 	// Prevents Bullet Rotated or Scaled by Socket
		// 	firePos.SetRotation(FPSCam->GetComponentRotation().Quaternion());
		// 	firePos.SetScale3D(FVector(1.0));

		// 	FVector firePos = GetSocketLocation(TEXT("FirePosition"));
		// 	FRotator fireRot = FPSCam->GetComponentRotation();
	}

	FTransform firePos = FTransform(FPSCam->GetComponentRotation(), GetSocketLocation(TEXT("FirePosition")));

	for (int32 i = 0; i < PelletCount; i++) {
		//AC_GunBullet* bullet = GetWorld()->SpawnActor<AC_GunBullet>(BulletFactory, firePos, fireRot, spawnParams);
		AC_GunBullet* bullet = GetWorld()->SpawnActorDeferred<AC_GunBullet>(BulletFactory, firePos, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		bullet->OnBulletInit(BulletDamage);

		AC_Pellet* pellet = Cast<AC_Pellet>(bullet);
		pellet->Dir = pellet->GetActorForwardVector();
		pellet->ApplySpread(SpreadPercent);

		// Both Codes Can be Used
		bullet->FinishSpawning(firePos);
//		UGameplayStatics::FinishSpawningActor(bullet, firePos);
	}
	UGameplayStatics::PlaySound2D(GetWorld(), BulletSound);
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
