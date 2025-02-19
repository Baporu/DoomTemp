// Fill out your copyright notice in the Description page of Project Settings.


#include "C_ProjectileBullet.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/C_Enemy.h"

AC_ProjectileBullet::AC_ProjectileBullet()
{
	// 1. �浹ü ����ϱ�
	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComp"));
	// 2. �浹 ������ ����
	CollisionComp->SetCollisionProfileName(TEXT("PlayerAttack"));
	// 3. �浹ü ũ�� ����
	CollisionComp->SetCapsuleHalfHeight(10.0f);
	CollisionComp->SetCapsuleRadius(5.0f);
	// 4. ��Ʈ�� ���
	SetRootComponent(CollisionComp);

	// 5. �ܰ� ������Ʈ ����ϱ�
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	// 6. �θ� ������Ʈ ����
	MeshComp->SetupAttachment(CollisionComp);
	// 7. �ܰ� ������Ʈ�� �浹 ��Ȱ��ȭ
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 8. �ܰ� ũ�� ����
	MeshComp->SetRelativeScale3D(FVector(0.25));

	// Add Overlap Function
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AC_GunBullet::OnBulletOverlap);
	// Add Overlap Function
	CollisionComp->OnComponentHit.AddDynamic(this, &AC_ProjectileBullet::OnBulletHit);
}

void AC_ProjectileBullet::BeginPlay()
{
	Super::BeginPlay();

	Dir = GetActorForwardVector();
}

void AC_ProjectileBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector movePos = GetActorLocation() + Dir * BulletSpeed * DeltaTime;
	SetActorLocation(movePos, true);

	LifeTime -= DeltaTime;

	if (LifeTime <= 0)
		Destroy();
}

void AC_ProjectileBullet::OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	AC_Enemy* enemy = Cast<AC_Enemy>(OtherActor);

	if (enemy == nullptr) {
		Destroy();
	}
}
