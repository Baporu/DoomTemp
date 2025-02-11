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

// AnyState인지 체크
bool AC_EWeapon::CanEquip()
{
	return true;
}

// 호출ㅁㅏㄴ
void AC_EWeapon::Equip()
{
	
}

// 호출만
void AC_EWeapon::BeginEquip()
{

}

// 호출만
void AC_EWeapon::EndEquip()
{

}

// 상태에는 우선순위가 있다
// AnyState : Hit, Dead가 우선순위가 제일 높음

bool AC_EWeapon::CanUnequip()
{
	// idle ㅅㅏㅇ태가 아니면
	// true 
	return true;
}

void AC_EWeapon::UnEquip()
{

}

