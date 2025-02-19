// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Pellet.h"
#include "Components/SphereComponent.h"
#include "Enemy/C_Enemy.h"
#include "Kismet/GameplayStatics.h"

AC_Pellet::AC_Pellet()
{
	// 1. 충돌체 등록하기
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	// 2. 충돌 프로필 설정
	CollisionComp->SetCollisionProfileName(TEXT("PlayerAttack"));
	// 3. 충돌체 크기 설정
	CollisionComp->SetSphereRadius(13.0f);
	// 4. 루트로 등록
	SetRootComponent(CollisionComp);

// 	// 5. 외관 컴포넌트 등록하기
// 	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
// 	// 6. 부모 컴포넌트 지정
// 	MeshComp->SetupAttachment(RootComponent);
// 	// 7. 외관 컴포넌트의 충돌 비활성화
// 	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
// 	// 8. 외관 크기 설정
// 	MeshComp->SetRelativeScale3D(FVector(0.25));

	// Add Overlap Function
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AC_GunBullet::OnBulletOverlap);
	// Add Overlap Function
	CollisionComp->OnComponentHit.AddDynamic(this, &AC_Pellet::OnBulletHit);
}

void AC_Pellet::BeginPlay()
{
	Super::BeginPlay();

	
}

void AC_Pellet::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PelletVFX, GetActorLocation());
}

void AC_Pellet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector movePos = GetActorLocation() + Dir * BulletSpeed * DeltaTime;
	SetActorLocation(movePos, true);

	LifeTime -= DeltaTime;

	if (LifeTime <= 0)
		Destroy();
}

void AC_Pellet::OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	AC_Enemy* enemy = Cast<AC_Enemy>(OtherActor);

	if (enemy == nullptr) {
		Destroy();
	}
}

void AC_Pellet::ApplySpread(float InSpread)
{
	Dir = ( GetActorForwardVector() + ( GetActorRightVector() * FMath::RandRange(-1.0, 1.0) * InSpread ) + ( GetActorUpVector() * FMath::RandRange(-1.0, 1.0) * InSpread ) ).GetSafeNormal();
}

