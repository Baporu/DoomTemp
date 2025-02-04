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

	/***** Enemy ���� üũ *****/
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


/***** Enemy ���� üũ *****/
#pragma region Check Enemy State
// Flinch �������� üũ
bool AC_Enemy::IsFlinched()
{
    if (HP > HPFlinched)
    {
        // �⺻ ���°� �ȴ�
        bIsFlinched = false;
        bIsStaggered = false;
        return false;
    }

    if (HP > HPStaggered)
    {
        // Flinch ���°� �ȴ�
        bIsFlinched = true;
        bIsStaggered = false;
        return true;
    }
}

// Stagger �������� üũ
bool AC_Enemy::IsStaggered()
{
    if (HP > HPStaggered)
    {
        // Flinch ���°� �ȴ�
        bIsStaggered = false;
        bIsFlinched = true;		// Stagger -> Flinch�� ���� ��ȯ�� �Ͼ���Ƿ�
        return false;
    }

    if (HP <= HPStaggered)
    {
        // Stagger ���°� �ȴ�
        bIsFlinched = false;		// Flinch -> Stagger�� ���� ��ȯ�� �Ͼ���Ƿ�
        bIsStaggered = true;
        return true;
    }
}
#pragma endregion

