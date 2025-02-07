//#include "Enemy/C_EWeaponComp.h"
//#include "C_Helpers.h"
//#include "Enemy/C_Enemy.h"
//#include "Enemy/C_EWeapon.h"
//
//UC_EWeaponComp::UC_EWeaponComp()
//{
//	PrimaryComponentTick.bCanEverTick = true;
//
//}
//
//
//void UC_EWeaponComp::BeginPlay()
//{
//	Super::BeginPlay();
//
//	Owner = Cast<AC_Enemy>(GetOwner());
//	CheckNull(Owner);
//
//	FActorSpawnParameters params;
//	params.Owner = Owner;
//	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
//	// 받은 타입을 Spawn
//	for(TSubclassOf<AC_EWeapon> weaponClass : WeaponClass)
//		if (!!weaponClass)
//		{
//			// weaponClass 타입으로 spawn
//			AC_EWeapon* weapon = Owner->GetWorld()->SpawnActor<AC_EWeapon>(weaponClass, params);
//			Weapons.Add(weapon);
//		}
//}
//
//
//void UC_EWeaponComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//}
//
//AC_EWeapon* UC_EWeaponComp::GetCurWeapon()
//{
//	CheckTrueResult(IsUnarmedMode(), nullptr);
//
//	return Weapons[(int32)WeaponType];
//}
//
//void UC_EWeaponComp::SetUnarmedMode()
//{
//	//////////////// 여기 왜 에러가 나는 걸까
//    CheckFalse(GetCurWeapon()->CanUnequip());
//    GetCurWeapon()->UnEquip();
//	ChangeType(EEWeaponType::MAX);
//}
//
//void UC_EWeaponComp::SetMode(EEWeaponType InType)
//{
//	if (WeaponType == InType)
//	{
//		SetUnarmedMode();
//		return;
//	}
//	else if (IsUnarmedMode() == true)
//	{
//		CheckFalse(GetCurWeapon()->CanUnequip());
//		GetCurWeapon()->UnEquip();
//	}
//
//	CheckNull(Weapons[(int32)InType]);
//	CheckFalse(Weapons[(int32)InType]->CanEquip());
//
//	Weapons[(int32)InType]->Equip();
//	ChangeType(InType);
//}
//
////////////////// 여기 왜 에러가 나는 걸까
////void UC_EWeaponComp::ChangeType(EEWeaponType InType)
////{
////	EEWeaponType type = WeaponType;
////	WeaponType = InType;
////	if(OnWeaponTypeChanged.IsBound())
////		OnWeaponTypeChanged.Broadcast(type, InType);
////}
//
