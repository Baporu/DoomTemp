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

	/***** Enemy ���� üũ *****/
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


/***** Enemy ���� üũ *****/
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


// ������ ó��
void AC_Enemy::OnDamaged(int32 InDamage, enum class EAttackType InAttackType)
{
    // 1. HP�� ��´�
    SetHP(InDamage);

    // 2. Attack Type�� �˷���
    FSM->SetAttackType(InAttackType);

    // 3. Enemy ���� ����
    CheckState();
}

// ��� ó��
void AC_Enemy::OnDead()
{
    // 1. �ٴڿ� �� VFX�� ��Ÿ����
    GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Cyan, FString("Enemy Dead !!!!!"));
    // 2. ���� �ִϸ��̼��� ����ȴ�


    // 3. ��� �ִ� Weapon�� Destroy�Ѵ�
    Weapon->WeaponDestroy();
}

