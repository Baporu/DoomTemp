#include "Enemy/C_EWeaponComp.h"
#include "C_Helpers.h"
#include "Enemy/C_Enemy.h"
#include "Enemy/C_EWeapon.h"

UC_EWeaponComp::UC_EWeaponComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UC_EWeaponComp::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AC_Enemy>(GetOwner());
	CheckNull(Owner);

	FActorSpawnParameters params;
	params.Owner = Owner;
	// Spawn �� �ɼ��� AlwaysSpawn���� ����
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	
	// ������ ���۵Ǹ� WeaponClasses�� ��� �ִ� ������� World�� ������Ŵ
	for(TSubclassOf<AC_EWeapon> weaponClass : WeaponClasses)
		if (!!weaponClass)
		{
			// weaponClass Ÿ������ spawn
			AC_EWeapon* weapon = Owner->GetWorld()->SpawnActor<AC_EWeapon>(weaponClass, params);
			Weapons.Add(weapon);
		}
}


void UC_EWeaponComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

// ���� ������ ���� return
AC_EWeapon* UC_EWeaponComp::GetCurWeapon()
{
	// IsUnarmedMode�� true�̸� ������ ���Ⱑ �����Ƿ� nullptr�� return
	CheckTrueResult(IsUnarmedMode(), nullptr);

	// ���� ���õ� Weapon�� return
	return Weapons[(int32)WeaponType];
}

// ���� ���� ����
void UC_EWeaponComp::SetUnarmedMode()
{
	// ���� ���Ⱑ ������ �� �ִ��� üũ
    CheckFalse(GetCurWeapon()->CanUnequip());

	// ���� ���� ���� ���� ����
    GetCurWeapon()->UnEquip();

	// ���� ���� ���� ���·� �ٲ�
	ChangeType(EEWeaponType::MAX);
}

// ���� ����
void UC_EWeaponComp::SetMode(EEWeaponType InType)
{
	/* ���� ����� �����Ϸ��� ���Ⱑ ���� ��� */
	if (WeaponType == InType)
	{
		// ���� ���� ���� �� return
		SetUnarmedMode();
		return;
	}
	/* ���� �����Ϸ��� ����� �ٸ� ���⸦ �����ϰ� �ִٸ� */
	else if (IsUnarmedMode() == false)
	{
		// ���� ���Ⱑ ������ �� �ִ��� üũ - Any State�� �ƴ����� üũ�ϴ� ��
		CheckFalse(GetCurWeapon()->CanUnequip());
		// ���� ���� ���� ���� ����
		GetCurWeapon()->UnEquip();
	}

	/* �����ϰ� �ִ� ���Ⱑ ���� ��� */
	// �����Ϸ��� ���Ⱑ �ִ��� üũ - BP�� Weapon data�� �ȳ־��� ���
	CheckNull(Weapons[(int32)InType]);

	// �����Ϸ��� ���⸦ ������ �� �ִ��� üũ - Any State�� �ƴ����� üũ�ϴ� ��
	CheckFalse(Weapons[(int32)InType]->CanEquip());

	// ���� ����
	Weapons[(int32)InType]->Equip();
	// ���� ����
	ChangeType(InType);
}

// ���� ���� ���� ���� ����
void UC_EWeaponComp::ChangeType(EEWeaponType InType)
{
	EEWeaponType type = WeaponType;
	// ���� ���� Ÿ�Կ��� �ٲ� ���� Ÿ������ ����
	WeaponType = InType;

	// ���� ���� Ÿ��, �ٲ� ���� Ÿ��
	if(OnWeaponTypeChanged.IsBound())
		OnWeaponTypeChanged.Broadcast(type, InType);
}

// ���� ���� ����
void UC_EWeaponComp::BeginEquip()
{
	CheckNull(GetCurWeapon());
	GetCurWeapon()->BeginEquip();
}

// ���� ���� ���� ���·� ����
void UC_EWeaponComp::EndEquip()
{
	CheckNull(GetCurWeapon());
	GetCurWeapon()->EndEquip();
}

