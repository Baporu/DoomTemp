#include "Enemy/C_EnemyA.h"
#include "Engine/SkeletalMesh.h"
#include "C_Helpers.h"
#include "Enemy/C_EnemyFSM.h"
#include "GameFramework/CharacterMovementComponent.h"

AC_EnemyA::AC_EnemyA()
{
    PrimaryActorTick.bCanEverTick = true;

    /***** Skeletal Mesh *****/
    ConstructorHelpers::FObjectFinder<USkeletalMesh> skeleton(L"/Script/Engine.SkeletalMesh'/Game/Zombies/Base_Mesh/Zombies_Body.Zombies_Body'");
    if (skeleton.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(skeleton.Object);
        GetMesh()->SetRelativeLocation( FVector(0.f, 0.f, -90.f) );
    }


    /***** Animation *****/
    ConstructorHelpers::FClassFinder<UAnimInstance> tmpAnim(L"/Script/Engine.AnimBlueprint'/Game/DYL/Blueprints/Enemy/Animations/ABP_EnemyA.ABP_EnemyA'_C");
    if (tmpAnim.Succeeded())
        GetMesh()->SetAnimInstanceClass(tmpAnim.Class);
}

void AC_EnemyA::BeginPlay()
{
    Super::BeginPlay();
}

void AC_EnemyA::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

//void AC_EnemyA::SetMeleeDamage()
//{
//    auto movement = FSM->GetEnemyMovement();
//
//    if (movement == EEnemyMovement::WALK)
//        MeleeDamage = MeleeDamageMax;
//    else if (movement == EEnemyMovement::FLINCH)
//        MeleeDamage = MeleeDamageFlinch;
//    else
//        MeleeDamage = 0;   // STAGGER || DEAD
//}
//
//
//void AC_EnemyA::SetSpeed()
//{
//    auto movement = FSM->GetEnemyMovement();
//
//    // 1. Enemy 상태에 따른 speed 변경
//    if (movement == EEnemyMovement::WALK)
//        Speed = SpeedMax;
//    else if (movement == EEnemyMovement::FLINCH)
//        Speed = SpeedFlinch;
//    else
//        Speed = 0;   // STAGGER || DEAD
//
//    // 2. 변경 내용 적용
//    GetCharacterMovement()->MaxWalkSpeed = Speed;
//}
