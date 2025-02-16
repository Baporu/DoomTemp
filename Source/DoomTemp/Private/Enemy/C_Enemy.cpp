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
    CheckSubState();
}


/***** Getters *****/
float AC_Enemy::GetMeleeRange()
{
	return MeleeRange;
}

// Enemy가 Flinch 상태이면 공격력 약화됨
int32 AC_Enemy::GetMeleeDamage()
{
    return MeleeDamage;
}


//float AC_Enemy::GetLongRange()
//{
//	return LongRange;
//}

// EnemyMovement에 따라 다른 Speed return
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


/***** Enemy 상태 체크 *****/
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


/***** 데미지 처리 *****/
void AC_Enemy::OnDamaged(int32 InDamage, enum class EAttackType InAttackType)
{
    // 1. HP를 깎는다
    SetHP(InDamage);

    // 2. Attack Type을 알려줌
    FSM->SetAttackType(InAttackType);

    // 3. Enemy 상태 변경
    FSM->SetEnemyState(EEnemyState::DAMAGE);
}


/***** 사망 처리 *****/
void AC_Enemy::OnDead()
{
    // 1. 바닥에 피 VFX가 나타난다
    GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Cyan, FString("Enemy Dead !!!!!"));
    // 2. 죽음 애니메이션이 재생된다


    // 3. 들고 있던 Weapon을 Destroy한다
    Weapon->WeaponDestroy();
}


// Enemy 상태에 따른 Speed 변경
void AC_Enemy::SetEnemySpeed()
{
    auto movement = FSM->GetEnemyMovement();

    // 1. Enemy 상태에 따른 speed 변경
    if (movement == EEnemyMovement::WALK)
        SetSpeed(SpeedMax);
    else if (movement == EEnemyMovement::FLINCH)
        SetSpeed(SpeedFlinch);
    else
        SetSpeed(0);   // STAGGER || DEAD

    // 2. 변경 내용 적용
    GetCharacterMovement()->MaxWalkSpeed = Speed;
}


// Enemy 상태에 따른 Melee Attack Damage 변경
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
    // 1. 맞는 애니메이션 재생

    // 2. 맞은 위치에 피 튀기는 VFX 나올 컴포넌트 부착

    // 3. 부착한 컴포넌트에서 맞은 부분의 normal 방향으로 피 튀기는 VFX 소환

    // 4. Enemy HP에 따른 Sub State 변경 및 죽음 처리
    CheckSubState();
}

void AC_Enemy::OnDamageGun()
{
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Get Damage GUN-----");
    // 1. 맞는 애니메이션 재생

    // 2. 맞은 위치에 피 튀기는 VFX 나올 컴포넌트 부착

    // 3. 부착한 컴포넌트에서 맞은 부분의 normal 방향으로 피 튀기는 VFX 소환

    // 4. Enemy HP에 따른 Sub State 변경 및 죽음 처리
    CheckSubState();
}

void AC_Enemy::OnDamageGloryKill()
{
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Get Damage GLORYKILL-----");
    // 1. 맞는 애니메이션 재생

    // 2. 맞은 위치에 피 뿜어져 나오는 VFX 나올 컴포넌트 부착

    // 3. 부착한 컴포넌트에서 맞은 부분의 normal 방향으로 피 뿜어져 나오는 VFX 소환

    // 4. Enemy HP에 따른 Sub State 변경 및 죽음 처리
    CheckSubState();
}

void AC_Enemy::OnDamageChainsaw()
{
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Get Damage CHAINSAW-----");

    // 1. 맞는 애니메이션 재생

    // 2. 맞은 위치에 피 뿜어져 나오는 VFX 나올 컴포넌트 부착

    // 3. 부착한 컴포넌트에서 맞은 부분의 normal 방향으로 피 뿜어져 나오는 VFX 소환

    // 4. Enemy HP에 따른 Sub State 변경 및 죽음 처리
    CheckSubState();
}