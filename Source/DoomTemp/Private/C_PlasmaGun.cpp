// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PlasmaGun.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Enemy/C_Enemy.h"

UC_PlasmaGun::UC_PlasmaGun()
{
	// Find Bullet Sound
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Script/Engine.SoundWave'/Game/SHS/Designs/SFX/SW_Scifi_Shoot.SW_Scifi_Shoot'"));

	if (tempSound.Succeeded())
		BulletSound = tempSound.Object;

	// Find Laser Sound
 	ConstructorHelpers::FObjectFinder<USoundBase> tempLaserSound(TEXT("/Script/Engine.SoundWave'/Game/SHS/Designs/SFX/SW_LaserPackage.SW_LaserPackage'"));
 
	if (tempSound.Succeeded()) {
		LaserSound = tempLaserSound.Object;
	}
}

void UC_PlasmaGun::BeginPlay()
{
	Super::BeginPlay();

	// Attach Laser Effects
	LaserComp = UGameplayStatics::SpawnEmitterAttached(LaserEffect, this, TEXT("FirePosition"));
	LaserHitComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaserHitEffect, FVector(0.0, 0.0, -1000.0));

	ToggleLaser(false);
	
	LaserTimer = LaserTime;
	LaserAttackTimer = LaserTime;
}

// Tick Function, Currently Not Used
// void UC_PlasmaGun::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
// 
// 	if (!bUseLaser)
// 		return;
// 
// 	if (CurrentAmmo <= 0 && bUsingMode) {
// 		ToggleLaser(false);
// 
// 		return;
// 	}
// 
// 	if (LaserTimer >= LaserTime) {
// 		CurrentAmmo -= 1;
// 
// 		UGameplayStatics::PlaySound2D(GetWorld(), BulletSound);
// 
// 		if (LaserTarget)
// 			LaserTarget->OnDamageProcess(LaserDamage, EAttackType::Gun);
// 
// 		LaserTime = 0.0f;
// 	}
// }

void UC_PlasmaGun::OnFire()
{
	if (CurrentAmmo <= 0) {
		if (bUsingMode)
			ToggleLaser(false);

		return;
	}

	// When Firing While Use Mode
	if (bUsingMode) {
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

		AC_Enemy* enemy = nullptr;
		FVector soundPos = endPos;

		LaserTimer += GetWorld()->GetDeltaSeconds();
		LaserAttackTimer += GetWorld()->GetDeltaSeconds();

		if (bHit) {
			LaserComp->SetBeamEndPoint(0, hitInfo.ImpactPoint);
			LaserHitComp->SetWorldLocation(hitInfo.Location);

			// Check Whether Hit Actor is Enemy or Not
			enemy = Cast<AC_Enemy>(hitInfo.GetActor());
			
			soundPos = hitInfo.ImpactPoint;
		}
		else {
			LaserComp->SetBeamEndPoint(0, endPos);
			LaserHitComp->SetWorldLocation(endPos);
		}

		if (LaserTimer >= 0.1f) {
			if (enemy && LaserAttackTimer >= LaserTime) {
				//UGameplayStatics::PlaySoundAtLocation(GetWorld(), LaserSound, soundPos);
				CurrentAmmo -= 1;

				enemy->OnDamageProcess(LaserDamage, EAttackType::Gun);

				LaserAttackTimer = 0.0f;
				LaserTime = 0.0f;
			}

			if (LaserTimer >= LaserTime) {
				CurrentAmmo -= 1;

				//UGameplayStatics::PlaySoundAtLocation(GetWorld(), LaserSound, soundPos);

				LaserTime = 0.0f;
			}

			UGameplayStatics::PlaySoundAtLocation(GetWorld(), LaserSound, soundPos, 0.3f, 1.0f, 17.0f);
		}

		ToggleLaser(true);
	}
	// When Firing Without Using Mode
	else {
		UGameplayStatics::PlaySound2D(GetWorld(), BulletSound, 0.2f);
		Super::OnFire();
		me->ShakePlayerCamera();
	}
}

void UC_PlasmaGun::OnUseMode()
{
	bUsingMode = !bUsingMode;

	if (!bUsingMode) {
		ToggleLaser(bUsingMode);
		
		me->SetFireRate(FireRate);
	}

	else
		me->SetFireRate(0);
}

void UC_PlasmaGun::ToggleLaser(bool InActive)
{
	bUseLaser = InActive;

	LaserComp->SetVisibility(InActive);
	LaserHitComp->SetVisibility(InActive);

	if (!InActive)
		LaserHitComp->SetWorldLocation(LaserHitComp->GetComponentLocation() + FVector(0.0, 0.0, -2000.0));
}
