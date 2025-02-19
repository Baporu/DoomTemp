// Fill out your copyright notice in the Description page of Project Settings.


#include "C_ProjectileBullet.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/C_Enemy.h"

AC_ProjectileBullet::AC_ProjectileBullet()
{
	// 1. 충돌체 등록하기
	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComp"));
	// 2. 충돌 프로필 설정
	CollisionComp->SetCollisionProfileName(TEXT("PlayerAttack"));
	// 3. 충돌체 크기 설정
	CollisionComp->SetCapsuleHalfHeight(10.0f);
	CollisionComp->SetCapsuleRadius(5.0f);
	// 4. 루트로 등록
	SetRootComponent(CollisionComp);

	// 5. 외관 컴포넌트 등록하기
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	// 6. 부모 컴포넌트 지정
	MeshComp->SetupAttachment(CollisionComp);
	// 7. 외관 컴포넌트의 충돌 비활성화
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 8. 외관 크기 설정
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
