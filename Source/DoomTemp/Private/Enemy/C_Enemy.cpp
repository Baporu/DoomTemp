#include "Enemy/C_Enemy.h"
#include "Enemy/C_EnemyFSM.h"
#include "GameFramework/Character.h"
#include <GameFramework/CharacterMovementComponent.h>


AC_Enemy::AC_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

    /***** Skeletal Mesh *****/
    ConstructorHelpers::FObjectFinder<USkeletalMesh> skeleton( L"/Script/Engine.SkeletalMesh'/Game/DYL/Designs/zombie-number-1-animated/source/zom_1.zom_1'" );
    if (skeleton.Succeeded())
    {
        GetMesh()->SetSkeletalMesh( skeleton.Object );
        GetMesh()->SetRelativeLocationAndRotation( FVector(0.f, 0.f, -90), FRotator(0.f, -90.f, 0.f) );
        GetMesh()->SetRelativeScale3D( FVector(10.f) );
    }

    /***** Speed *****/
    GetCharacterMovement()->MaxWalkSpeed = Speed;

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

