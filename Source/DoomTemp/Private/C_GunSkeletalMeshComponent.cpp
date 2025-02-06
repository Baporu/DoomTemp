// Fill out your copyright notice in the Description page of Project Settings.


#include "C_GunSkeletalMeshComponent.h"
#include "C_GunBullet.h"

UC_GunSkeletalMeshComponent::UC_GunSkeletalMeshComponent()
{
	// Load Skeletal Mesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	// If Skeletal Mesh Loaded
	if (TempMesh.Succeeded()) {
		// Set Loaded Mesh
		SetSkeletalMesh(TempMesh.Object);
	}

	// Disable Collision
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UC_GunSkeletalMeshComponent::InputFire()
{
	FTransform firePos = GetSocketTransform(TEXT("FirePosition"));
	firePos.SetScale3D(FVector(1.0));

	GetWorld()->SpawnActor<AC_GunBullet>(BulletFactory, firePos);
}
