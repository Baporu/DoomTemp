#include "Enemy/C_Enemy.h"
#include "C_Helpers.h"
#include "GameFramework/Character.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Enemy/C_EnemyFSM.h"


AC_Enemy::AC_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

    /***** Speed *****/
    GetCharacterMovement()->MaxWalkSpeed = Speed;

	/***** FSM *****/
    C_Helpers::CreateActorComponent<UC_EnemyFSM>(this, &FSM, "FSM");

    /***** Weapon *****/
    //C_Helpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
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
    HP += InHP;
    CheckState();
}


/***** Enemy ���� üũ *****/
// Flinch �������� üũ
void AC_Enemy::CheckState()
{
    if (HP > HPStaggered)
    {
        if (HP <= HPFlinched)
        {
            // Flinch ���°� �ȴ�
            bIsFlinched = true;
            bIsStaggered = false;
        }
        else
        {
            // �⺻ ���°� �ȴ�
            bIsFlinched = false;
            bIsStaggered = false;
        }
    }
    else
    {
        if(HP == 0)
            bIsDead = true;
        else
        {
            // Stagger ���°� �ȴ�
            bIsFlinched = false;
            bIsStaggered = true;
        }
    }
}

