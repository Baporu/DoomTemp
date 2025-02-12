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

	// 모드를 사용 중일 때
	if (bUsingMode) {

	}
	// 그냥 발사만 할 때
	else {
		FTransform firePos = GetSocketTransform(TEXT("FirePosition"));
		// 무기의 크기에 따라 Socket의 Transform도 달라지는 경우가 있는데,
		// 그럴 경우 총알의 크기가 같이 변경되는 일을 방지하기 위해 크기를 1.0으로 고정한다.
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
