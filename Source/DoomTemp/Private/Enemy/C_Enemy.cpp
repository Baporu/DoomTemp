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
    CheckSubState();
}


/***** Getters *****/
float AC_Enemy::GetMeleeRange()
{
	return MeleeRange;
}

// Enemy�� Flinch �����̸� ���ݷ� ��ȭ��
int32 AC_Enemy::GetMeleeDamage()
{
    return MeleeDamage;
}


//float AC_Enemy::GetLongRange()
//{
//	return LongRange;
//}

// EnemyMovement�� ���� �ٸ� Speed return
float AC_Enemy::GetSpeed()
{
    return Speed;
}


/***** Setters *****/
void AC_Enemy::SetHP(int32 InVal)
{
    HP -= InVal;
}


void AC_Enemy::SetMeleeDamage(int32 InVal)
{
    MeleeDamage = InVal;
}


void AC_Enemy::SetSpeed(float InVal)
{
    Speed = InVal;
}


/***** Enemy ���� üũ *****/
void AC_Enemy::CheckSubState()
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


/***** ������ ó�� *****/
void AC_Enemy::OnDamaged(int32 InDamage, enum class EAttackType InAttackType)
{
    // 1. HP�� ��´�
    SetHP(InDamage);

    // 2. Attack Type�� �˷���
    FSM->SetAttackType(InAttackType);

    // 3. Enemy ���� ����
    FSM->SetEnemyState(EEnemyState::DAMAGE);
}


/***** ��� ó�� *****/
void AC_Enemy::OnDead()
{
    // 1. �ٴڿ� �� VFX�� ��Ÿ����
    GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Cyan, FString("Enemy Dead !!!!!"));
    // 2. ���� �ִϸ��̼��� ����ȴ�


    // 3. ��� �ִ� Weapon�� Destroy�Ѵ�
    Weapon->WeaponDestroy();
}


// Enemy ���¿� ���� Speed ����
void AC_Enemy::SetEnemySpeed()
{
    auto movement = FSM->GetEnemyMovement();

    // 1. Enemy ���¿� ���� speed ����
    if (movement == EEnemyMovement::WALK)
        SetSpeed(SpeedMax);
    else if (movement == EEnemyMovement::FLINCH)
        SetSpeed(SpeedFlinch);
    else
        SetSpeed(0);   // STAGGER || DEAD

    // 2. ���� ���� ����
    GetCharacterMovement()->MaxWalkSpeed = Speed;
}


// Enemy ���¿� ���� Melee Attack Damage ����
void AC_Enemy::ChangeMeleeDamage()
{
    auto movement = FSM->GetEnemyMovement();

    if (movement == EEnemyMovement::WALK)
        SetMeleeDamage(MeleeDamageMax);
    else if (movement == EEnemyMovement::FLINCH)
        SetMeleeDamage(MeleeDamageFlinch);
    else
        SetMeleeDamage(0);   // STAGGER || DEAD
}


/***** Damage Events *****/
void AC_Enemy::OnDamageFist()
{
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Get Damage FIST-----");
    // 1. �´� �ִϸ��̼� ���

    // 2. ���� ��ġ�� �� Ƣ��� VFX ���� ������Ʈ ����

    // 3. ������ ������Ʈ���� ���� �κ��� normal �������� �� Ƣ��� VFX ��ȯ

    // 4. Enemy HP�� ���� Sub State ���� �� ���� ó��
    CheckSubState();
}

void AC_Enemy::OnDamageGun()
{
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Get Damage GUN-----");
    // 1. �´� �ִϸ��̼� ���

    // 2. ���� ��ġ�� �� Ƣ��� VFX ���� ������Ʈ ����

    // 3. ������ ������Ʈ���� ���� �κ��� normal �������� �� Ƣ��� VFX ��ȯ

    // 4. Enemy HP�� ���� Sub State ���� �� ���� ó��
    CheckSubState();
}

void AC_Enemy::OnDamageGloryKill()
{
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Get Damage GLORYKILL-----");
    // 1. �´� �ִϸ��̼� ���

    // 2. ���� ��ġ�� �� �վ��� ������ VFX ���� ������Ʈ ����

    // 3. ������ ������Ʈ���� ���� �κ��� normal �������� �� �վ��� ������ VFX ��ȯ

    // 4. Enemy HP�� ���� Sub State ���� �� ���� ó��
    CheckSubState();
}

void AC_Enemy::OnDamageChainsaw()
{
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Get Damage CHAINSAW-----");

    // 1. �´� �ִϸ��̼� ���

    // 2. ���� ��ġ�� �� �վ��� ������ VFX ���� ������Ʈ ����

    // 3. ������ ������Ʈ���� ���� �κ��� normal �������� �� �վ��� ������ VFX ��ȯ

    // 4. Enemy HP�� ���� Sub State ���� �� ���� ó��
    CheckSubState();
}