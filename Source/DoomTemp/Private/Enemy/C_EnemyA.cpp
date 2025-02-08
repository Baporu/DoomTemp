#include "Enemy/C_EnemyA.h"
#include "Engine/SkeletalMesh.h"

AC_EnemyA::AC_EnemyA()
{
    PrimaryActorTick.bCanEverTick = true;

    /***** Skeletal Mesh *****/
    ConstructorHelpers::FObjectFinder<USkeletalMesh> skeleton(L"/Script/Engine.SkeletalMesh'/Game/DYL/Designs/zombie-number-1-animated/source/zom_1.zom_1'");
    if (skeleton.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(skeleton.Object);
        GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90), FRotator(0.f, -90.f, 0.f));
        GetMesh()->SetRelativeScale3D(FVector(10.f));
    }


    /***** Animation *****/
    ConstructorHelpers::FClassFinder<UAnimInstance> tmpClass(L"/Game/DYL/Designs/zombie-number-1-animated/source/zom_1.zom_1");
    if (tmpClass.Succeeded())
        GetMesh()->SetAnimInstanceClass(tmpClass.Class);
}

void AC_EnemyA::BeginPlay()
{
    Super::BeginPlay();
}

void AC_EnemyA::Tick(float DeltaTime)
{
    
}
