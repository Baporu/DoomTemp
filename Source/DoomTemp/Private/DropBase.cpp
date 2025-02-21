// Fill out your copyright notice in the Description page of Project Settings.


#include "DropBase.h"
#include "Components/BoxComponent.h"
#include "C_PlayerCharacter.h"
#include "C_GunSkeletalMeshComponent.h"

// Sets default values
ADropBase::ADropBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// 1. �浹ü ����ϱ�
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	// 2. �浹 ������ ����
	CollisionComp->SetCollisionProfileName(TEXT("Drop"));
	// 3. �浹ü ũ�� ����
	CollisionComp->SetBoxExtent(FVector(5.0));
	// 4. ��Ʈ�� ���
	SetRootComponent(CollisionComp);

	// 5. �ܰ� ������Ʈ ����ϱ�
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	// 6. �θ� ������Ʈ ����
	MeshComp->SetupAttachment(RootComponent);
	// 7. �ܰ� ������Ʈ�� �浹 ��Ȱ��ȭ
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 8. �ܰ� ũ�� ����
	MeshComp->SetRelativeScale3D(FVector(0.1));

	// Add Overlap Function
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ADropBase::OnDropOverlap);
}

// Called when the game starts or when spawned
void ADropBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADropBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ADropBase::OnDropOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AC_PlayerCharacter* player = Cast<AC_PlayerCharacter>(OtherActor);

	if (!player)
		return;

	switch (DropType)
	{
		case EDropType::Health:
			player->CurrentHP += HealValue;

			if (player->CurrentHP > player->MaxHP)
				player->CurrentHP = player->MaxHP;

			break;

		case EDropType::Saw:
			player->CurrentFuel += HealValue;

			if (player->CurrentFuel > player->MaxFuel)
				player->CurrentFuel = player->MaxFuel;

			break;

		case EDropType::Plasma:
			if (player->PlasmaMesh)
				player->PlasmaMesh->IncreaseAmmo(HealValue);
			
			break;

		case EDropType::Sniper:
			if (player->SniperMesh)
				player->SniperMesh->IncreaseAmmo(HealValue);

			break;

		case EDropType::Shotgun:
			if (player->ShotgunMesh)
				player->ShotgunMesh->IncreaseAmmo(HealValue);

			break;
	}

	Destroy();
}

