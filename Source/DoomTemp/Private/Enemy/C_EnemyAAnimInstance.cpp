#include "Enemy/C_EnemyAAnimInstance.h"
#include "Enemy/C_EnemyA.h"
#include "C_Helpers.h"
#include "Enemy/C_EWeaponComp.h"
#include "Enemy/C_EWeaponScratch.h"
#include "Components/SphereComponent.h"

void UC_EnemyAAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation( DeltaSeconds );
    Enemy = Cast<AC_EnemyA>(TryGetPawnOwner());
    CheckNull(Enemy);
}

void UC_EnemyAAnimInstance::AnimNotify_EAttackStart()
{
    // Weapon Scratch의 player channel과의 collision을 overlap으로 변경
    ChangePlayerChannelCollision(ECR_Overlap);
}

void UC_EnemyAAnimInstance::AnimNotify_EAttackEnd()
{
    // Weapon Scratch의 player channel과의 collision을 Ignore로 변경
    ChangePlayerChannelCollision(ECR_Ignore);

    // Attack이 끝났을 때 실행할 함수 실행
    Enemy->GetEnemyFSM()->OnAttackEnd();
}

void UC_EnemyAAnimInstance::AnimNotify_DeadEnd()
{
    // Dead 애니메이션이 끝남
    if( AC_EnemyA* enemy = Cast<AC_EnemyA>( TryGetPawnOwner() ) )
        enemy->GetEnemyFSM()->onDeadEnd();
}

void UC_EnemyAAnimInstance::AnimNotify_ChangeDamageRateStart()
{
    // 해당 Notify가 실행되면 실행 중인 montage의 실행 속도를 0.8로 바꿔줌
    if(Enemy->GetCurrentMontage() != nullptr)
        Montage_SetPlayRate(Enemy->GetCurrentMontage(), 0.85f);
}

void UC_EnemyAAnimInstance::AnimNotify_ChangeDamageRateEnd()
{
    if (Enemy->GetCurrentMontage() != nullptr)
        Montage_SetPlayRate(Enemy->GetCurrentMontage(), 1.f);
}

void UC_EnemyAAnimInstance::AnimNotify_SpawnEnd()
{
    // Attack이 끝났을 때 실행할 함수 실행
    Enemy->GetEnemyFSM()->SetEnemyState(EEnemyState::IDLE);
}

void UC_EnemyAAnimInstance::ChangePlayerChannelCollision(ECollisionResponse InResponse)
{
    // Enemy가 장착 중인 무기들을 가져온다
    TArray<class AC_EWeapon*> weapons = Enemy->GetWeaponComps()->GetWeapons();

    for (AC_EWeapon* weapon : weapons)
    {
        if (weapon != nullptr)
        {
            // Scratch Weapon이 있다면 
            if (weapon->IsA<AC_EWeaponScratch>())
            {
                // player channel과의 collision을 InResponse로 변경
                AC_EWeaponScratch* scratch = Cast<AC_EWeaponScratch>(weapon);
                scratch->GetScratchComp()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, InResponse);
            }
        }
    }
}
