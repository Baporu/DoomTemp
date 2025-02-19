// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BounceBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Enemy/C_Enemy.h"

AC_BounceBullet::AC_BounceBullet()
{
	// 1. 충돌체 등록하기
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	// 2. 충돌 프로필 설정
	CollisionComp->SetCollisionProfileName(TEXT("PlayerAttack"));
	// 3. 충돌체 크기 설정
	CollisionComp->SetSphereRadius(13.0f);
	// 4. 루트로 등록
	SetRootComponent(CollisionComp);

	// 5. 외관 컴포넌트 등록하기
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	// 6. 부모 컴포넌트 지정
	MeshComp->SetupAttachment(RootComponent);
	// 7. 외관 컴포넌트의 충돌 비활성화
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 8. 외관 크기 설정
	MeshComp->SetRelativeScale3D(FVector(0.25));

	// Add Overlap Function
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AC_GunBullet::OnBulletOverlap);


	// 1. 발사체 등록하기
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	// 2. Movement 컴포넌트가 갱신시킬 컴포넌트 지정
	MovementComp->SetUpdatedComponent(CollisionComp);
	// 3. 초기 속도 설정
	MovementComp->InitialSpeed = BulletSpeed;
	// 4. 최대 속도 설정
	MovementComp->MaxSpeed = BulletSpeed;
	// 5. 반동 여부 설정
	MovementComp->bShouldBounce = true;
	// 6. 반동 값 설정
	MovementComp->Bounciness = 0.3f;

	// 생명 시간 주기
	InitialLifeSpan = LifeTime;
}
