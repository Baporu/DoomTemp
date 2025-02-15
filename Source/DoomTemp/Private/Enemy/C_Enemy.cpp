#include "Enemy/C_Enemy.h"
#include "C_Helpers.h"
#include "GameFramework/Character.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Enemy/C_EnemyFSM.h"
#include "C_PlayerCharacter.h"
#include "Enemy/C_EWeaponComp.h"


AC_Enemy::AC_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

    /***** Speed *****/
    GetCharacterMovement()->MaxWalkSpeed = Speed;

	/***** FSM *****/
    C_Helpers::CreateActorComponent<UC_EnemyFSM>(this, &FSM, "FSM");

    /***** Weapon *****/
    C_Helpers::CreateActorComponent<UC_EWeaponComp>(this, &Weapon, "Weapon");
}

void AC_Enemy::BeginPlay()
{
	Super::BeginPlay();
    
}

void AC_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/***** Enemy 상태 체크 *****/
    CheckState();
}

float AC_Enemy::GetMeleeRange()
{
	return MeleeRange;
}

int32 AC_Enemy::GetMleeDamage()
{
    return MeleeDamage;
}

//float AC_Enemy::GetLongRange()
//{
//	return LongRange;
//}



float AC_Enemy::GetSpeed()
{
    return Speed;
}

void AC_Enemy::SetHP(int32 InHP)
{
    HP -= InHP;
}


/***** Enemy 상태 체크 *****/
void AC_Enemy::CheckState()
{
    /* Enemy Move : Walk > Flinch > Stagger > Dead */

    if (HP > HPFlinched)
    {
        FSM->SetEnemyMovement(EEnemyMovement::WALK);
    }
    else if (HP > HPStaggered)
    {
        FSM->SetEnemyMovement(EEnemyMovement::FLINCH);

    }
    else if (HP > 0.f)
    {
        FSM->SetEnemyMovement(EEnemyMovement::STAGGER);
    }
    else
    {
        FSM->SetEnemyState(EEnemyState::DEAD);
    }
}


// 데미지 처리
void AC_Enemy::OnDamaged(int32 InDamage, enum class EAttackType InAttackType)
{
    // 1. HP를 깎는다
    SetHP(InDamage);

    // 2. Attack Type을 알려줌
    FSM->SetAttackType(InAttackType);

    // 3. Enemy 상태 변경
    CheckState();
}

// 사망 처리
void AC_Enemy::OnDead()
{
    // 1. 바닥에 피 VFX가 나타난다
    GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Cyan, FString("Enemy Dead !!!!!"));
    // 2. 죽음 애니메이션이 재생된다


    // 3. 들고 있던 Weapon을 Destroy한다
    Weapon->WeaponDestroy();
}

