#include "Enemy/C_EWeapon.h"
#include "Components/SceneComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Enemy/C_Enemy.h"
#include "Components/SkeletalMeshComponent.h"

AC_EWeapon::AC_EWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	//C_Helpers::CreateComponent<USceneComponent>(this, &Root, "Root");
    //Root = CreateDefaultSubobject<USceneComponent>( L"Root" );
    //SetRootComponent( Root );

	//C_Helpers::CreateComponent<USkeletalMesh>(this, &Mesh, "Mesh", Root);
	//Mesh = CreateDefaultSubobject<USkeletalMeshComponent>( L"Mesh" );
	//Mesh->SetupAttachment( Root );
}

void AC_EWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	Owner = Cast<AC_Enemy>(GetOwner());

	if(WeaponSocketName.IsValid())
		AttachToComponent( Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), WeaponSocketName);
}

void AC_EWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// AnyState���� üũ
bool AC_EWeapon::CanEquip()
{
	return true;
}

// ȣ�⤱����
void AC_EWeapon::Equip()
{
	
}

// ȣ�⸸
void AC_EWeapon::BeginEquip()
{

}

// ȣ�⸸
void AC_EWeapon::EndEquip()
{

}

// ���¿��� �켱������ �ִ�
// AnyState : Hit, Dead�� �켱������ ���� ����

bool AC_EWeapon::CanUnequip()
{
	// idle �������°� �ƴϸ�
	// true 
	return true;
}

void AC_EWeapon::UnEquip()
{

}

