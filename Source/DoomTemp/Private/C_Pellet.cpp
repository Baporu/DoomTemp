// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Pellet.h"
#include "Components/SphereComponent.h"
#include "Enemy/C_Enemy.h"
#include "Kismet/GameplayStatics.h"

AC_Pellet::AC_Pellet()
{
	// 1. �浹ü ����ϱ�
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	// 2. �浹 ������ ����
	CollisionComp->SetCollisionProfileName(TEXT("PlayerAttack"));
	// 3. �浹ü ũ�� ����
	CollisionComp->SetSphereRadius(13.0f);
	// 4. ��Ʈ�� ���
	SetRootComponent(CollisionComp);

// 	// 5. �ܰ� ������Ʈ ����ϱ�
// 	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
// 	// 6. �θ� ������Ʈ ����
// 	MeshComp->SetupAttachment(RootComponent);
// 	// 7. �ܰ� ������Ʈ�� �浹 ��Ȱ��ȭ
// 	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
// 	// 8. �ܰ� ũ�� ����
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

