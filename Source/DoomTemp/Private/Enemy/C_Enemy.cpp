#include "Enemy/C_Enemy.h"
#include "C_Helpers.h"
#include "GameFramework/Character.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Enemy/C_EnemyFSM.h"
#include "C_PlayerCharacter.h"
#include "Enemy/C_EWeaponComp.h"
#include "Enemy/C_EnemyAAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"


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
    //CheckSubState();
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


UC_EnemyFSM* AC_Enemy::GetEnemyFSM()
{
    return FSM;
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
        FSM->SetEnemyMovement(EEnemyMovement::WALK);
    else if (HP > HPStaggered)
        FSM->SetEnemyMovement(EEnemyMovement::FLINCH);
    else if (HP > 0.f)
        FSM->SetEnemyMovement(EEnemyMovement::STAGGER);
    else
    {
        FSM->SetEnemyState(EEnemyState::DEAD);
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    // �ִϸ��̼� ���� ����ȭ
    FSM->SetAnimState(FSM->GetEnemyState());
    FSM->SetAnimSubStateMovement(FSM->GetEnemyMovement());

    // Speed ����
    SetEnemySpeed();

    // ���ݷ� ����
    ChangeMeleeDamage();
}


/***** ������ ó�� *****/
void AC_Enemy::OnDamageProcess(int32 InDamage, enum class EAttackType InAttackType)
{
    // HP�� ��´�
    SetHP(InDamage);

    // ��ã�� ��� ����
    FSM->GetMyAI()->StopMovement();

    // Enemy ���� ����
    //CheckSubState();


    // Attack Type�� ���� �ִϸ��̼� �� ����Ʈ ����
    switch (InAttackType)
    {
    	case EAttackType::Fist:
    		OnDamageFist();
    		break;
    	case EAttackType::Gun:
    		OnDamageGun();
    		break;
    	case EAttackType::GloryKill:
    		OnDamageGloryKill();
    		break;
    	case EAttackType::Chainsaw:
    		OnDamageChainsaw();
    		break;
    }
}


/***** ��� ó�� *****/
void AC_Enemy::OnDead()
{
    GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Cyan, FString("Enemy Dead !!!!!"));

    // 1. �ٴڿ� �� VFX�� ��Ÿ����
    
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
    else
        SetSpeed(0);   // FLINCH, STAGGER, ATTACK, DEAD

    // 2. ���� ���� ����
    GetCharacterMovement()->MaxWalkSpeed = Speed;

    // Debug
    FString DebugMessage = FString::Printf(TEXT(">>>>> Speed Change: %f"), Speed);
    GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Orange, DebugMessage);
    // 250217 - Sub state check �����ϰ� speed �����ϰ� �ߴ��� ����� �����
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
        SetMeleeDamage(0);   // STAGGER, DEAD
}


/***** Damage Events *****/
void AC_Enemy::OnDamageFist()
{
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Get Damage FIST-----");

    // 1. �´� �ִϸ��̼� ���
    SectionName = FString::Printf(TEXT("Damage%d"), 0);
    FSM->PlayEnemyMontage(&SectionName);

    // 2. ���� ��ġ�� �� Ƣ��� VFX ���� ������Ʈ ����

    // 3. ������ ������Ʈ���� ���� �κ��� normal �������� �� Ƣ��� VFX ��ȯ

    // 4. Enemy HP�� ���� Sub State ���� �� ���� ó��
    //CheckSubState();
}

void AC_Enemy::OnDamageGun()
{
    //GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Get Damage GUN-----");

    // 1. �´� �ִϸ��̼� ���
    SectionName = FString::Printf(TEXT("Damage%d"), 0);
    FSM->PlayEnemyMontage(&SectionName);

    // 2. ���� ��ġ�� �� Ƣ��� VFX ���� ������Ʈ ����

    // 3. ������ ������Ʈ���� ���� �κ��� normal �������� �� Ƣ��� VFX ��ȯ

    // 4. Enemy HP�� ���� Sub State ���� �� ���� ó��
    //CheckSubState();
}

void AC_Enemy::OnDamageGloryKill()
{
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Get Damage GLORYKILL-----");

    // 1. �´� �ִϸ��̼� ���
    SectionName = FString::Printf(TEXT("Damage%d"), 0);
    FSM->PlayEnemyMontage(&SectionName);

    // 2. ���� ��ġ�� �� �վ��� ������ VFX ���� ������Ʈ ����

    // 3. ������ ������Ʈ���� ���� �κ��� normal �������� �� �վ��� ������ VFX ��ȯ

    // 4. Enemy HP�� ���� Sub State ���� �� ���� ó��
    //CheckSubState();
}

void AC_Enemy::OnDamageChainsaw()
{
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Get Damage CHAINSAW-----");

    // 1. �´� �ִϸ��̼� ���
    SectionName = FString::Printf(TEXT("Damage%d"), 0);
    FSM->PlayEnemyMontage(&SectionName);

    // 2. ���� ��ġ�� �� �վ��� ������ VFX ���� ������Ʈ ����

    // 3. ������ ������Ʈ���� ���� �κ��� normal �������� �� �վ��� ������ VFX ��ȯ

    // 4. Enemy HP�� ���� Sub State ���� �� ���� ó��
    //CheckSubState();
}