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


	// 1. �浹ü ����ϱ�
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	// 2. �浹 ������ ����
	CollisionComp->SetCollisionProfileName(TEXT("PlayerAttack"));
	// 3. �浹ü ũ�� ����
	CollisionComp->SetSphereRadius(20.0f);
	// 4. ��Ʈ�� ���
	SetRootComponent(CollisionComp);

	// Add Overlap Function
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AC_GunBullet::OnBulletOverlap);

	// 5. �ܰ� ������Ʈ ����ϱ�
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	// 6. �θ� ������Ʈ ����
	MeshComp->SetupAttachment(RootComponent);
	// 7. �ܰ� ������Ʈ�� �浹 ��Ȱ��ȭ
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 8. �ܰ� ũ�� ����
	MeshComp->SetRelativeScale3D(FVector(0.5f));

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

