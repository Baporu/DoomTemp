// Fill out your copyright notice in the Description page of Project Settings.


#include "C_SniperGun.h"
#include "C_GunBullet.h"

UC_SniperGun::UC_SniperGun()
{
	// Load Skeletal Mesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/SHS/Designs/SV-98_Sniper/source/vector.vector'"));

	// If Skeletal Mesh Loaded
	if (TempMesh.Succeeded()) {
		// Set Loaded Mesh
		SetSkeletalMesh(TempMesh.Object);
	}
}

// void UC_SniperGun::InputFire()
// {
// 	for (int i = 0; i < 5; i++) {
// 		FTransform firePos = GetSocketTransform(FName(TEXT("FirePosition"), i));
// 
// 		firePos.SetScale3D(FVector(1.0));
// 
// 		GetWorld()->SpawnActor<AC_GunBullet>(BulletFactory, firePos);
// 	}
// }
