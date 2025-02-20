// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PlasmaGun.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

void UC_PlasmaGun::BeginPlay()
{
	Super::BeginPlay();

	// Attach Laser Effects
	LaserComp = UGameplayStatics::SpawnEmitterAttached(LaserEffect, this, TEXT("FirePosition"));
	LaserHitComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaserHitEffect, FVector(0.0, 0.0, -1000.0));

	ToggleLaser(false);
}

void UC_PlasmaGun::OnFire()
{
	if (CurrentAmmo <= 0)
		return;

	// When Firing While Use Mode
	if (bUsingMode) {
		me->SetFireRate(0);

		// Variables for LineTrace
		FVector startPos = FPSCam->GetComponentLocation();
		FVector endPos = startPos + FPSCam->GetForwardVector() * 5000.0f;
		FHitResult hitInfo;
		// Collision Option Parameter
		FCollisionQueryParams params;
		
		// Make LineTrace Ignore Player
		params.AddIgnoredActor(GetOwner());

		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECollisionChannel::ECC_Visibility, params);

		LaserComp->SetBeamSourcePoint(0, GetSocketLocation(TEXT("FirePosition")), 0);

		if (bHit) {
			LaserComp->SetBeamEndPoint(0, hitInfo.ImpactPoint);
			LaserHitComp->SetWorldLocation(hitInfo.Location);

// 			// 맞은 대상이 Enemy인지 아닌지 판별
// 			AC_Enemy* enemy = Cast<AC_Enemy>(hitInfo.GetActor());
// 
// 			if (enemy) {
// 				enemy->OnDamageProcess(SnipeDamage, EAttackType::Gun);
// 			}
		}
		else {
			LaserComp->SetBeamEndPoint(0, endPos);
			LaserHitComp->SetWorldLocation(endPos);
		}

		ToggleLaser(true);

// 		// Debug LineTrace
// 		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Blue, false, 2.0f, 0, 1.0f);
	}
	// When Firing Without Using Mode
	else {
		Super::OnFire();
	}
}

void UC_PlasmaGun::OnUseMode()
{
	bUsingMode = !bUsingMode;

	if (!bUsingMode) {
		ToggleLaser(bUsingMode);
		
		me->SetFireRate(FireRate);
	}
}

void UC_PlasmaGun::ToggleLaser(bool InActive)
{
	LaserComp->SetVisibility(InActive);
	LaserHitComp->SetVisibility(InActive);
}
