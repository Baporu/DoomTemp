#include "Enemy/C_EWeaponScratch.h"
#include "C_Helpers.h"
#include "Components/SphereComponent.h"
#include "C_PlayerCharacter.h"
#include "Enemy/C_Enemy.h"

AC_EWeaponScratch::AC_EWeaponScratch()
{
    /***** Scratch Comp *****/
    C_Helpers::CreateComponent<USphereComponent>(this, &ScratchComp, "Scratch_L", RootComponent);
    ScratchComp->SetCollisionProfileName( FName("EnemyAttack") );
    ScratchComp->SetRelativeScale3D( FVector(0.4f) );
    ScratchComp->OnComponentBeginOverlap.AddDynamic(this, &AC_EWeaponScratch::OnEWeaponScratchOverlap);
    //ScratchComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    /***** Equip *****/
    // Weapon을 부착할 소켓 이름
    WeaponSocketName = "Scratch_L";
}


/***** Attack *****/
void AC_EWeaponScratch::OnEWeaponScratchOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 1. 충돌한 character가 player인지 확인한다
    AC_PlayerCharacter* player = Cast<AC_PlayerCharacter>(OtherActor);
    CheckNull(player);

    // 2. player에게 데미지를 입힌다
    player->PlayerHit(Owner->GetMeleeDamage());
}

