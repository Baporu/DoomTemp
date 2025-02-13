// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BounceBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Enemy/C_Enemy.h"

AC_BounceBullet::AC_BounceBullet()
{
	// 1. 발사체 등록하기
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	// 2. Movement 컴포넌트가 갱신시킬 컴포넌트 지정
	MovementComp->SetUpdatedComponent(CollisionComp);
	// 3. 초기 속도 설정
	MovementComp->InitialSpeed = 5000.0f;
	// 4. 최대 속도 설정
	MovementComp->MaxSpeed = 5000.0f;
	// 5. 반동 여부 설정
	MovementComp->bShouldBounce = true;
	// 6. 반동 값 설정
	MovementComp->Bounciness = 0.3f;

	// 생명 시간 주기
	InitialLifeSpan = 2.0f;

	// Add Overlap Function
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AC_BounceBullet::OnBulletOverlap);
}
