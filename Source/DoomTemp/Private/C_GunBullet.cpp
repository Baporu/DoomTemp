// Fill out your copyright notice in the Description page of Project Settings.


#include "C_GunBullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Enemy/C_Enemy.h"

// Sets default values
AC_GunBullet::AC_GunBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// 1. 충돌체 등록하기
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	// 2. 충돌 프로필 설정
	CollisionComp->SetCollisionProfileName(TEXT("PlayerAttack"));
	// 3. 충돌체 크기 설정
	CollisionComp->SetSphereRadius(20.0f);
	// 4. 루트로 등록
	SetRootComponent(CollisionComp);

	// Add Overlap Function
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AC_GunBullet::OnBulletOverlap);

	// 5. 외관 컴포넌트 등록하기
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	// 6. 부모 컴포넌트 지정
	MeshComp->SetupAttachment(RootComponent);
	// 7. 외관 컴포넌트의 충돌 비활성화
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 8. 외관 크기 설정
	MeshComp->SetRelativeScale3D(FVector(0.5f));

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
}

// Called when the game starts or when spawned
void AC_GunBullet::BeginPlay()
{
	Super::BeginPlay();
	
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
		//OtherActor->Destroy();

		Destroy();
	}
}

