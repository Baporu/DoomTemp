// Fill out your copyright notice in the Description page of Project Settings.


#include "C_SniperGun.h"
#include "C_GunBullet.h"
#include "Kismet/GameplayStatics.h"
#include "C_PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Enemy/C_Enemy.h"

UC_SniperGun::UC_SniperGun()
{
	// Find Bullet Sound
// 	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Script/Engine.SoundWave'/Game/SniperGun/Rifle.Rifle'"));
// 
// 	if (tempSound.Succeeded())
// 		BulletSound = tempSound.Object;

	// Find Snipe Sound
//  	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT(""));
//  
//  	if (tempSound.Succeeded())
//  		SnipeSound = tempSound.Object;
}

void UC_SniperGun::BeginPlay()
{
	Super::BeginPlay();

	SniperUI = CreateWidget<UUserWidget>(GetWorld(), SniperUIFactory);
	CrossHairUI = CreateWidget<UUserWidget>(GetWorld(), CrossHairUIFactory);
	CrossHairUI->AddToViewport();
}

void UC_SniperGun::OnFire()
{
	if (CurrentAmmo <= 0)
		return;

	// If Using Aim Mode
	if (bUsingMode) {
		if (CurrentAmmo < 4)
			return;

		CurrentAmmo -= 4;

		// Variables for LineTrace
		FVector startPos = FPSCam->GetComponentLocation();
		FVector endPos = startPos + FPSCam->GetForwardVector() * 5000.0f;
		FHitResult hitInfo;
		// Collision Option Parameter
		FCollisionQueryParams params;

		// Make LineTrace Ignore Player
		params.AddIgnoredActor(GetOwner());

		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECollisionChannel::ECC_Visibility, params);
		FVector soundPos = endPos;

		if (bHit) {
			FTransform bulletTrans;
			bulletTrans.SetLocation(hitInfo.ImpactPoint);

			soundPos = hitInfo.ImpactPoint;

			// Bullet FX
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SniperEffect, bulletTrans);

			auto hitComp = hitInfo.GetComponent();

			if (hitComp && hitComp->IsSimulatingPhysics()) {
				FVector dir = (endPos - startPos).GetSafeNormal();
				FVector force = dir * hitComp->GetMass() * 50000;

				hitComp->AddForceAtLocation(force, hitInfo.ImpactPoint);
			}

			// 맞은 대상이 Enemy인지 아닌지 판별
			AC_Enemy* enemy = Cast<AC_Enemy>(hitInfo.GetActor());

			if (enemy) {
				enemy->OnDamageProcess(SnipeDamage, EAttackType::Gun, hitInfo.ImpactPoint, hitInfo.ImpactNormal);
			}
		}

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), BulletSound, soundPos, 0.6f);
		me->SetFireRate(FireRate * 5);

		// Debug LineTrace
// 		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Blue, false, 1.5f, 0, 1.0f);
	}

	else {
		UGameplayStatics::PlaySound2D(GetWorld(), BulletSound, 0.4f);
		Super::OnFire();
	}
}

void UC_SniperGun::OnUseMode()
{
	bUsingMode = !bUsingMode;

	if (bUsingMode) {
		// 스나이퍼 조준 UI 등록
		SniperUI->AddToViewport();
		// 카메라의 시야각인 FOV(Field Of View) 조절
		FPSCam->SetFieldOfView(45.0f);

		// 일반 조준 UI 제거
		CrossHairUI->RemoveFromParent();
	}
	else {
		// 스나이퍼 조준 UI 제거
		SniperUI->RemoveFromParent();
		// 카메라의 FOV를 원래대로 복원
		FPSCam->SetFieldOfView(90.0f);

		// 일반 조준 UI 등록
		CrossHairUI->AddToViewport();
		me->SetFireRate(FireRate);
	}
}

