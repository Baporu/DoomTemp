#include "Enemy/C_EnemyAAnimInstance.h"
#include "Enemy/C_EnemyA.h"
#include "C_Helpers.h"

void UC_EnemyAAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation( DeltaSeconds );

    AC_EnemyA* enemyA = Cast<AC_EnemyA>(TryGetPawnOwner());
    CheckNull(enemyA);

    FVector velocity = enemyA->GetVelocity();
    FVector forwardVector = enemyA->GetActorForwardVector();

    // 앞/뒤 이동
    Speed = FVector::DotProduct(velocity, forwardVector);
}
