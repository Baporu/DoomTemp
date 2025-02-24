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
	CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	// 3. �浹ü ũ�� ����
	CollisionComp->SetBoxExtent(FVector(27.0, 25.0, 40.0));
	// 4. ��Ʈ�� ���
	SetRootComponent(CollisionComp);

	// Add Overlap Function
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ADropBase::OnDropOverlap);
}

// Called when the game starts or when spawned
void ADropBase::BeginPlay()
{
	Super::BeginPlay();
	

	Dir = (GetActorForwardVector() * FMath::RandRange(-1.0, 1.0) + GetActorRightVector() * FMath::RandRange(-1.0, 1.0) + GetActorUpVector()).GetSafeNormal2D();
}

// Called every frame
void ADropBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FlyTimer += DeltaTime;

	if (FlyTimer >= FlyTime) {
		if (!bEnabledCollision) {
			CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

			bEnabledCollision = !bEnabledCollision;
		}

		return;
	}

	SetActorLocation(GetActorLocation() + Dir, true);
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

