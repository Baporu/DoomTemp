#include "Enemy/C_EWeapon.h"
#include "Components/SceneComponent.h"
#include "Enemy/C_Enemy.h"
//#include "Components/SkeletalMeshComponent.h"
#include "C_Helpers.h"


AC_EWeapon::AC_EWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

    C_Helpers::CreateComponent<USceneComponent>(this, &Root, "Root");
    //C_Helpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh", Root);
}

void AC_EWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	Owner = Cast<AC_Enemy>(GetOwner());

	BeginEquip();
}

void AC_EWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// ���� ���� ���� �������� üũ
bool AC_EWeapon::CanEquip()
{
	bool b = false;

	// 3�� �� false�� �� b = false;
	// 3�� �� �ϳ��� true�� b = true;
	b |= bEquipping; // ������
	b |= bReload; // ������
	b |= bFiring; // �ѽ���ֳ�
	
	return !b;	// b == false�� true ��ȯ
}

// ���� ���� �ϰ� �ִ� ���·� ����
void AC_EWeapon::Equip()
{
	bEquipping = true;

	//if(!!EquipMontage)
	//	Owner->PlayAnimMontage(EquipMontage, EquipMontagePlayRate);
}

// ���� ���� ����
void AC_EWeapon::BeginEquip()
{
	if (WeaponSocketName.IsValid())
		AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), WeaponSocketName);
}

// ���� ���� ���� ���·� ����
void AC_EWeapon::EndEquip()
{
	bEquipping = false;
}

// ���� ���� ���� ���� �������� üũ
bool AC_EWeapon::CanUnequip()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;

	return !b;
}

// ���� ���� ���� ����
void AC_EWeapon::UnEquip()
{
	if (WeaponSocketName.IsValid())
		AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), WeaponSocketName);
}

//void AC_EWeapon::OnEnemyDestroy()
//{
//	Destroy();
//}
