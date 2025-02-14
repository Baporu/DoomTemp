// Fill out your copyright notice in the Description page of Project Settings.


#include "C_GunSkeletalMeshComponent.h"
#include "C_GunBullet.h"

void UC_GunSkeletalMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;
}

void UC_GunSkeletalMeshComponent::OnFire()
{
	FTransform firePos = GetSocketTransform(TEXT("FirePosition"));
	firePos.SetRotation(GetOwner()->GetActorRotation().Quaternion());
	// ������ ũ�⿡ ���� Socket�� Transform�� �޶����� ��찡 �ִµ�,
	// �׷� ��� �Ѿ��� ũ�Ⱑ ���� ����Ǵ� ���� �����ϱ� ���� ũ�⸦ 1.0���� �����Ѵ�.
	firePos.SetScale3D(FVector(1.0));

	AC_GunBullet* bullet = GetWorld()->SpawnActor<AC_GunBullet>(BulletFactory, firePos);
	bullet->OnBulletInit(BulletDamage);

	--CurrentAmmo;
}

void UC_GunSkeletalMeshComponent::OnStartMode()
{

}

void UC_GunSkeletalMeshComponent::OnUseMode()
{
	bUsingMode = !bUsingMode;
}

void UC_GunSkeletalMeshComponent::OnEndMode()
{

}

void UC_GunSkeletalMeshComponent::IncreaseAmmo() {
	CurrentAmmo += 10;

	if (CurrentAmmo > MaxAmmo)
		CurrentAmmo = MaxAmmo;
}

float UC_GunSkeletalMeshComponent::GetFireRate()
{
	return FireRate;
}
