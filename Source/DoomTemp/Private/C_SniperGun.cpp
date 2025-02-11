// Fill out your copyright notice in the Description page of Project Settings.


#include "C_SniperGun.h"
#include "C_GunBullet.h"
#include "Kismet/GameplayStatics.h"
#include "C_PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

void UC_SniperGun::BeginPlay()
{
	Super::BeginPlay();

	AC_PlayerCharacter* player = Cast<AC_PlayerCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AC_PlayerCharacter::StaticClass()));
	FPSCam = player->GetCameraComponent();

	SniperUI = CreateWidget<UUserWidget>(GetWorld(), SniperUIFactory);
	CrossHairUI = CreateWidget<UUserWidget>(GetWorld(), CrossHairUIFactory);
	CrossHairUI->AddToViewport();
}

void UC_SniperGun::OnFire()
{
	bIsFire = !bIsFire;

	if (bIsFire) {
		SniperAttack();
	}
}

void UC_SniperGun::OnUseMode()
{
	bUsingMode = !bUsingMode;

	if (bUsingMode) {
		// �������� ���� UI ���
		SniperUI->AddToViewport();
		// ī�޶��� �þ߰��� FOV(Field Of View) ����
		FPSCam->SetFieldOfView(45.0f);

		// �Ϲ� ���� UI ����
		CrossHairUI->RemoveFromParent();
	}
	else {
		// �������� ���� UI ����
		SniperUI->RemoveFromParent();
		// ī�޶��� FOV�� ������� ����
		FPSCam->SetFieldOfView(90.0f);

		// �Ϲ� ���� UI ���
		CrossHairUI->AddToViewport();
	}
}

void UC_SniperGun::SniperAttack()
{
	if (CurrentAmmo <= 0)
		return;

	// If Using Aim Mode
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

		if (bHit) {
			FTransform bulletTrans;
			bulletTrans.SetLocation(hitInfo.ImpactPoint);

			// Bullet FX
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SniperEffect, bulletTrans);

			auto hitComp = hitInfo.GetComponent();

			if (hitComp && hitComp->IsSimulatingPhysics()) {
				FVector dir = (endPos - startPos).GetSafeNormal();
				FVector force = dir * hitComp->GetMass() * 50000;

				hitComp->AddForceAtLocation(force, hitInfo.ImpactPoint);
			}

			// Check Whether Hit Actor is Enemy or Not
// 			auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
// 
// 			if (enemy) {
// 				
// 			}
		}

		// Debug LineTrace
		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Blue, false, 2.0f, 0, 1.0f);
	}

	else {
		Super::OnFire();
	}
}

