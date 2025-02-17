// Fill out your copyright notice in the Description page of Project Settings.


#include "C_GunSkeletalMeshComponent.h"
#include "C_GunBullet.h"
#include "Kismet/GameplayStatics.h"
#include "C_PlayerCharacter.h"
#include "Camera/CameraComponent.h"

void UC_GunSkeletalMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	AC_PlayerCharacter* player = Cast<AC_PlayerCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AC_PlayerCharacter::StaticClass()));
	FPSCam = player->GetCameraComponent();

	CurrentAmmo = MaxAmmo;
}

void UC_GunSkeletalMeshComponent::OnFire()
{
	FTransform firePos = GetSocketTransform(TEXT("FirePosition"));
	firePos.SetRotation(FPSCam->GetComponentRotation().Quaternion());
	// 무기의 크기에 따라 Socket의 Transform도 달라지는 경우가 있는데,
	// 그럴 경우 총알의 크기가 같이 변경되는 일을 방지하기 위해 크기를 1.0으로 고정한다.
	firePos.SetScale3D(FVector(1.0));

	AC_GunBullet* bullet = GetWorld()->SpawnActor<AC_GunBullet>(BulletFactory, firePos);
	bullet->OnBulletInit(BulletDamage);

	--CurrentAmmo;
}

void UC_GunSkeletalMeshComponent::OnStartMode()
{

}

void UC_GunSkeletalMeshComponent::OnUseMode()
{
	bUsingMode = !bUsingMode;
}

void UC_GunSkeletalMeshComponent::OnEndMode()
{

}

void UC_GunSkeletalMeshComponent::IncreaseAmmo() {
	CurrentAmmo += 10;

	if (CurrentAmmo > MaxAmmo)
		CurrentAmmo = MaxAmmo;
}

void UC_GunSkeletalMeshComponent::OnGunChanged() {
	// If Weapon is Using Mode, Reset Mode Before Being Changed
	if (bUsingMode)
		OnUseMode();
}

float UC_GunSkeletalMeshComponent::GetFireRate()
{
	return FireRate;
}
