#include "Enemy/C_Enemy.h"
#include "Enemy/C_EnemyFSM.h"

AC_Enemy::AC_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

	/***** FSM *****/
	FSM = CreateDefaultSubobject<UC_EnemyFSM>( L"FSM" );
}

void AC_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AC_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/***** Enemy 상태 체크 *****/
	//CheckIsFlinched( HP );
	//CheckIsStaggered( HP );
}

float AC_Enemy::GetMeleeRange()
{
	return MeleeRange;
}

float AC_Enemy::GetLongRange()
{
	return LongRange;
}


/***** Enemy 상태 체크 *****/
#pragma region Check Enemy State
// Flinch 상태인지 체크
bool AC_Enemy::IsFlinched()
{
    if (HP > HPFlinched)
    {
        // 기본 상태가 된다
        bIsFlinched = false;
        bIsStaggered = false;
        return false;
    }

    if (HP > HPStaggered)
    {
        // Flinch 상태가 된다
        bIsFlinched = true;
        bIsStaggered = false;
        return true;
    }
}

// Stagger 상태인지 체크
bool AC_Enemy::IsStaggered()
{
    if (HP > HPStaggered)
    {
        // Flinch 상태가 된다
        bIsStaggered = false;
        bIsFlinched = true;		// Stagger -> Flinch로 상태 전환이 일어났으므로
        return false;
    }

    if (HP <= HPStaggered)
    {
        // Stagger 상태가 된다
        bIsFlinched = false;		// Flinch -> Stagger로 상태 전환이 일어났으므로
        bIsStaggered = true;
        return true;
    }
}
#pragma endregion

