#include "Enemy/C_EnemyAAnimInstance.h"
#include "Enemy/C_EnemyA.h"
#include "C_Helpers.h"

void UC_EnemyAAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation( DeltaSeconds );

}

void UC_EnemyAAnimInstance::AnimNotify_EAttackEnd()
{
    GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, TEXT(">>>>> Enemy Attack End"));
    UE_LOG(LogTemp, Warning, TEXT(">>>>> Enemy Attack End"));

    AC_EnemyA* enemy = Cast<AC_EnemyA>(TryGetPawnOwner());

    if (enemy != nullptr)
    {
        // Attack이 끝났을 때 실행할 함수 실행
        enemy->GetEnemyFSM()->OnAttackEnd();
    }
}

void UC_EnemyAAnimInstance::AnimNotify_SpawnEnd()
{
    GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, TEXT(">>>>> Enemy Attack End"));
    UE_LOG(LogTemp, Warning, TEXT(">>>>> Enemy Spawn End"));

    AC_EnemyA* enemy = Cast<AC_EnemyA>(TryGetPawnOwner());

    if (enemy != nullptr)
    {
        // Attack이 끝났을 때 실행할 함수 실행
        enemy->GetEnemyFSM()->SetEnemyState(EEnemyState::IDLE);
    }
}
