// Fill out your copyright notice in the Description page of Project Settings.


#include "C_GunSkeletalMeshComponent.h"
#include "C_GunBullet.h"
#include "Kismet/GameplayStatics.h"
#include "C_PlayerAnimInstance.h"

UC_GunSkeletalMeshComponent::UC_GunSkeletalMeshComponent()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryComponentTick.bCanEverTick = true;

	// Find Bullet Sound
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Script/Engine.SoundWave'/Game/SniperGun/Rifle.Rifle'"));

	if (tempSound.Succeeded())
		BulletSound = tempSound.Object;
}

void UC_GunSkeletalMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;
}

void UC_GunSkeletalMeshComponent::OnFire()
{
	me->Anim->PlayShootAnim();

// 	FTransform firePos = GetSocketTransform(TEXT("FirePosition"));
// 	firePos.SetRotation(FPSCam->GetComponentRotation().Quaternion());
// 	// Prevents Bullet Scaled by Socket
// 	firePos.SetScale3D(FVector(1.0));

//	FVector firePos = GetSocketLocation(TEXT("FirePosition"));
//	FRotator fireRot = FPSCam->GetComponentRotation();
//	AC_GunBullet* bullet = GetWorld()->SpawnActor<AC_GunBullet>(BulletFactory, firePos, fireRot);

	FTransform firePos = FTransform(FPSCam->GetComponentRotation(), GetSocketLocation(TEXT("FirePosition")));
	AC_GunBullet* bullet = GetWorld()->SpawnActorDeferred<AC_GunBullet>(BulletFactory, firePos, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	bullet->OnBulletInit(BulletDamage);
	bullet->FinishSpawning(firePos);
//	UGameplayStatics::FinishSpawningActor(bullet, firePos);

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

void UC_GunSkeletalMeshComponent::IncreaseAmmo(int32 amount) {
	CurrentAmmo += amount;

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

// int32 UC_GunSkeletalMeshComponent::GetBulletDamage()
// {
// 	return BulletDamage;
// }

