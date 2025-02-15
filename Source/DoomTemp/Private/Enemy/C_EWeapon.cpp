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

// 무기 장착 가능 상태인지 체크
bool AC_EWeapon::CanEquip()
{
	bool b = false;

	// 3개 다 false일 때 b = false;
	// 3개 중 하나라도 true면 b = true;
	b |= bEquipping; // 장착중
	b |= bReload; // 장전중
	b |= bFiring; // 총쏘고있냐
	
	return !b;	// b == false면 true 반환
}

// 무기 장착 하고 있는 상태로 변경
void AC_EWeapon::Equip()
{
	bEquipping = true;

	//if(!!EquipMontage)
	//	Owner->PlayAnimMontage(EquipMontage, EquipMontagePlayRate);
}

// 무기 장착 시작
void AC_EWeapon::BeginEquip()
{
	if (WeaponSocketName.IsValid())
		AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), WeaponSocketName);
}

// 무기 장착 끝난 상태로 변경
void AC_EWeapon::EndEquip()
{
	bEquipping = false;
}

// 무기 장착 해제 가능 상태인지 체크
bool AC_EWeapon::CanUnequip()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;

	return !b;
}

// 무기 장착 해제 실행
void AC_EWeapon::UnEquip()
{
	if (WeaponSocketName.IsValid())
		AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), WeaponSocketName);
}

//void AC_EWeapon::OnEnemyDestroy()
//{
//	Destroy();
//}
