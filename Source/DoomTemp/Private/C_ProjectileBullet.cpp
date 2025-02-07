// Fill out your copyright notice in the Description page of Project Settings.


#include "C_ProjectileBullet.h"
#include "Components/SphereComponent.h"
#include "Enemy/C_Enemy.h"

AC_ProjectileBullet::AC_ProjectileBullet()
{
	// Add Overlap Function
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AC_ProjectileBullet::OnBulletOverlap);
	CollisionComp->OnComponentHit.AddDynamic(this, &AC_ProjectileBullet::OnBulletHit);
}

void AC_ProjectileBullet::Tick(float DeltaTime)
{
	FVector movePos = GetActorLocation() + GetActorForwardVector() * BulletSpeed * DeltaTime;
	SetActorLocation(movePos, true);

	LifeTimer -= DeltaTime;

	if (LifeTimer <= 0)
		Destroy();
}

void AC_ProjectileBullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AC_Enemy* enemy = Cast<AC_Enemy>(OtherActor);

	if (enemy != nullptr) {
		//OtherActor->Destroy();

		//Destroy();
	}
}

void AC_ProjectileBullet::OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	AC_Enemy* enemy = Cast<AC_Enemy>(OtherActor);

	if (enemy == nullptr) {
		Destroy();
	}
}
