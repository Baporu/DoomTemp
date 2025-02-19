// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BounceBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Enemy/C_Enemy.h"

AC_BounceBullet::AC_BounceBullet()
{
	// 1. �浹ü ����ϱ�
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	// 2. �浹 ������ ����
	CollisionComp->SetCollisionProfileName(TEXT("PlayerAttack"));
	// 3. �浹ü ũ�� ����
	CollisionComp->SetSphereRadius(13.0f);
	// 4. ��Ʈ�� ���
	SetRootComponent(CollisionComp);

	// 5. �ܰ� ������Ʈ ����ϱ�
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	// 6. �θ� ������Ʈ ����
	MeshComp->SetupAttachment(RootComponent);
	// 7. �ܰ� ������Ʈ�� �浹 ��Ȱ��ȭ
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 8. �ܰ� ũ�� ����
	MeshComp->SetRelativeScale3D(FVector(0.25));

	// Add Overlap Function
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AC_GunBullet::OnBulletOverlap);


	// 1. �߻�ü ����ϱ�
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	// 2. Movement ������Ʈ�� ���Ž�ų ������Ʈ ����
	MovementComp->SetUpdatedComponent(CollisionComp);
	// 3. �ʱ� �ӵ� ����
	MovementComp->InitialSpeed = BulletSpeed;
	// 4. �ִ� �ӵ� ����
	MovementComp->MaxSpeed = BulletSpeed;
	// 5. �ݵ� ���� ����
	MovementComp->bShouldBounce = true;
	// 6. �ݵ� �� ����
	MovementComp->Bounciness = 0.3f;

	// ���� �ð� �ֱ�
	InitialLifeSpan = LifeTime;
}
