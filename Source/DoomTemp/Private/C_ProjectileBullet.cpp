// Fill out your copyright notice in the Description page of Project Settings.


#include "C_ProjectileBullet.h"
#include "Components/SphereComponent.h"
#include "Enemy/C_Enemy.h"
#include "Kismet/GameplayStatics.h"

AC_ProjectileBullet::AC_ProjectileBullet()
{
	// 1. 충돌체 등록하기
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	// 2. 충돌 프로필 설정
	CollisionComp->SetCollisionProfileName(TEXT("PlayerAttack"));
	// 3. 충돌체 크기 설정
	CollisionComp->SetSphereRadius(13.0);
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
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletVFX, HitResult.ImpactPoint);

		Destroy();
	}
}
