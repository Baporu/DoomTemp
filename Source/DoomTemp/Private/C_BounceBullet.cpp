// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BounceBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Enemy/C_Enemy.h"

AC_BounceBullet::AC_BounceBullet()
{
	// 1. �߻�ü ����ϱ�
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	// 2. Movement ������Ʈ�� ���Ž�ų ������Ʈ ����
	MovementComp->SetUpdatedComponent(CollisionComp);
	// 3. �ʱ� �ӵ� ����
	MovementComp->InitialSpeed = 5000.0f;
	// 4. �ִ� �ӵ� ����
	MovementComp->MaxSpeed = 5000.0f;
	// 5. �ݵ� ���� ����
	MovementComp->bShouldBounce = true;
	// 6. �ݵ� �� ����
	MovementComp->Bounciness = 0.3f;

	// ���� �ð� �ֱ�
	InitialLifeSpan = 2.0f;

	// Add Overlap Function
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AC_BounceBullet::OnBulletOverlap);
}

void AC_BounceBullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AC_Enemy* enemy = Cast<AC_Enemy>(OtherActor);

	if (enemy != nullptr) {
		enemy->SetHP(Damage);

		Destroy();
	}
}
