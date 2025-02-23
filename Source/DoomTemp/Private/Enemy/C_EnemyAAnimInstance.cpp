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
    // Weapon Scratch�� player channel���� collision�� overlap���� ����
    ChangePlayerChannelCollision(ECR_Overlap);
}

void UC_EnemyAAnimInstance::AnimNotify_EAttackEnd()
{
    // Weapon Scratch�� player channel���� collision�� Ignore�� ����
    ChangePlayerChannelCollision(ECR_Ignore);

    // Attack�� ������ �� ������ �Լ� ����
    Enemy->GetEnemyFSM()->OnAttackEnd();
}

void UC_EnemyAAnimInstance::AnimNotify_DeadEnd()
{
    // Dead �ִϸ��̼��� ����
    if( AC_EnemyA* enemy = Cast<AC_EnemyA>( TryGetPawnOwner() ) )
        enemy->GetEnemyFSM()->onDeadEnd();
}

void UC_EnemyAAnimInstance::AnimNotify_ChangeDamageRateStart()
{
    // �ش� Notify�� ����Ǹ� ���� ���� montage�� ���� �ӵ��� 0.8�� �ٲ���
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
    // Attack�� ������ �� ������ �Լ� ����
    Enemy->GetEnemyFSM()->SetEnemyState(EEnemyState::IDLE);
}

void UC_EnemyAAnimInstance::ChangePlayerChannelCollision(ECollisionResponse InResponse)
{
    // Enemy�� ���� ���� ������� �����´�
    TArray<class AC_EWeapon*> weapons = Enemy->GetWeaponComps()->GetWeapons();

    for (AC_EWeapon* weapon : weapons)
    {
        if (weapon != nullptr)
        {
            // Scratch Weapon�� �ִٸ� 
            if (weapon->IsA<AC_EWeaponScratch>())
            {
                // player channel���� collision�� InResponse�� ����
                AC_EWeaponScratch* scratch = Cast<AC_EWeaponScratch>(weapon);
                scratch->GetScratchComp()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, InResponse);
            }
        }
    }
}
