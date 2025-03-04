// Fill out your copyright notice in the Description page of Project Settings.


#include "C_GunBullet.h"
#include "Enemy/C_Enemy.h"
#include "C_PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AC_GunBullet::AC_GunBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AC_GunBullet::BeginPlay()
{
	Super::BeginPlay();
	

// 	AC_PlayerCharacter* player = Cast<AC_PlayerCharacter>(GetInstigator());
// 
// 	if (!player) {
// 		UE_LOG(LogTemp, Warning, TEXT("Bullet BeginPlay(): Instigator Cast Failed!"));
// 		return;
// 	}
// 
// 	UC_GunSkeletalMeshComponent* gun = player->GetCurrentGun();
// 
// 	if (!gun) {
// 		UE_LOG(LogTemp, Warning, TEXT("Bullet BeginPlay(): Get Gun Failed!"));
// 		return;
// 	}
// 
// 	UE_LOG(LogTemp, Warning, TEXT("Bullet BeginPlay(): Get Instigator Succeeded!"));
// 	Damage = gun->GetBulletDamage();
}

// Called every frame
void AC_GunBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AC_GunBullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AC_Enemy* enemy = Cast<AC_Enemy>(OtherActor);

	if (enemy != nullptr) {
		enemy->OnDamageProcess(Damage, EAttackType::Gun, SweepResult.ImpactPoint, SweepResult.ImpactNormal);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletVFX, SweepResult.ImpactPoint);

		Destroy();
	}
}

void AC_GunBullet::OnBulletInit(int32 InBulletDamage)
{
	Damage = InBulletDamage;
}

