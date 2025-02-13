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
	// Spawn 시 옵션을 AlwaysSpawn으로 설정
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	
	// 게임이 시작되면 WeaponClasses에 들어 있는 무기들을 World에 스폰시킴
	for(TSubclassOf<AC_EWeapon> weaponClass : WeaponClasses)
		if (!!weaponClass)
		{
			// weaponClass 타입으로 spawn
			AC_EWeapon* weapon = Owner->GetWorld()->SpawnActor<AC_EWeapon>(weaponClass, params);
			Weapons.Add(weapon);
		}
}


void UC_EWeaponComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

// 현재 장착한 무기 return
AC_EWeapon* UC_EWeaponComp::GetCurWeapon()
{
	// IsUnarmedMode가 true이면 리턴할 무기가 없으므로 nullptr을 return
	CheckTrueResult(IsUnarmedMode(), nullptr);

	// 현재 선택된 Weapon을 return
	return Weapons[(int32)WeaponType];
}

// 무기 장착 해제
void UC_EWeaponComp::SetUnarmedMode()
{
	// 현재 무기가 해제될 수 있는지 체크
    CheckFalse(GetCurWeapon()->CanUnequip());

	// 현재 장착 중인 무기 해제
    GetCurWeapon()->UnEquip();

	// 현재 무기 없음 상태로 바꿈
	ChangeType(EEWeaponType::MAX);
}

// 무기 장착
void UC_EWeaponComp::SetMode(EEWeaponType InType)
{
	/* 현재 무기와 장착하려는 무기가 같은 경우 */
	if (WeaponType == InType)
	{
		// 무기 장착 해제 후 return
		SetUnarmedMode();
		return;
	}
	/* 현재 장착하려는 무기와 다른 무기를 장착하고 있다면 */
	else if (IsUnarmedMode() == false)
	{
		// 현재 무기가 해제될 수 있는지 체크 - Any State가 아닌지를 체크하는 것
		CheckFalse(GetCurWeapon()->CanUnequip());
		// 현재 장착 중인 무기 해제
		GetCurWeapon()->UnEquip();
	}

	/* 장착하고 있는 무기가 없는 경우 */
	// 장착하려는 무기가 있는지 체크 - BP에 Weapon data를 안넣어준 경우
	CheckNull(Weapons[(int32)InType]);

	// 장착하려는 무기를 장착할 수 있는지 체크 - Any State가 아닌지를 체크하는 것
	CheckFalse(Weapons[(int32)InType]->CanEquip());

	// 무기 장착
	Weapons[(int32)InType]->Equip();
	// 무기 변경
	ChangeType(InType);
}

// 장착 중인 무기 상태 변경
void UC_EWeaponComp::ChangeType(EEWeaponType InType)
{
	EEWeaponType type = WeaponType;
	// 기존 무기 타입에서 바뀐 무기 타입으로 변경
	WeaponType = InType;

	// 기존 무기 타입, 바뀐 무기 타입
	if(OnWeaponTypeChanged.IsBound())
		OnWeaponTypeChanged.Broadcast(type, InType);
}

// 무기 장착 시작
void UC_EWeaponComp::BeginEquip()
{
	CheckNull(GetCurWeapon());
	GetCurWeapon()->BeginEquip();
}

// 무기 장착 끝난 상태로 변경
void UC_EWeaponComp::EndEquip()
{
	CheckNull(GetCurWeapon());
	GetCurWeapon()->EndEquip();
}

