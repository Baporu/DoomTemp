#include "Enemy/C_EnemyA.h"
#include "Engine/SkeletalMesh.h"

AC_EnemyA::AC_EnemyA()
{
    PrimaryActorTick.bCanEverTick = true;

    /****** ¿Ü°ü *****/
    ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletal( L"" );
    if (skeletal.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(skeletal.Object);
        //GetMesh()->SetRelativeLocationAndRotation( FVector(), FRotator() );
    }
}

void AC_EnemyA::BeginPlay()
{
    Super::BeginPlay();
}

void AC_EnemyA::Tick(float DeltaTime)
{
    
}
