// Fill out your copyright notice in the Description page of Project Settings.


#include "C_ProjectileBullet.h"
#include "Components/SphereComponent.h"
#include "Enemy/C_Enemy.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "C_PlayerCharacter.h"
#include "C_GunSkeletalMeshComponent.h"

AC_ProjectileBullet::AC_ProjectileBullet()
{
	// Add Overlap Function
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AC_ProjectileBullet::OnBulletOverlap);
	CollisionComp->OnComponentHit.AddDynamic(this, &AC_ProjectileBullet::OnBulletHit);
}

void AC_ProjectileBullet::BeginPlay()
{
	Super::BeginPlay();

	AC_PlayerCharacter* player = Cast<AC_PlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (player) {
		FVector firePos = player->ShotgunMesh->GetSocketLocation(TEXT("FirePosition"));
		firePos -= player->GetActorForwardVector() * 20;
		Dir = (GetActorLocation() - firePos);
	}
}

void AC_ProjectileBullet::Tick(float DeltaTime)
{
// 	Dir = GetActorForwardVector();
	FVector movePos = GetActorLocation() + Dir * BulletSpeed * DeltaTime;
	SetActorLocation(movePos, true);

	LifeTimer -= DeltaTime;

	if (LifeTimer <= 0)
		Destroy();
}

void AC_ProjectileBullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AC_Enemy* enemy = Cast<AC_Enemy>(OtherActor);

	if (enemy != nullptr) {
		enemy->SetHP(Damage);

		Destroy();
	}
}

void AC_ProjectileBullet::OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	AC_Enemy* enemy = Cast<AC_Enemy>(OtherActor);

	if (enemy == nullptr) {
		Destroy();
	}
}
