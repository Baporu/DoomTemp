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

    // ������ ���� player���� �浹�� ignore�� ����
    ScratchComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

    ScratchComp->SetRelativeScale3D( FVector(0.4f) );
    ScratchComp->OnComponentBeginOverlap.AddDynamic(this, &AC_EWeaponScratch::OnEWeaponScratchOverlap);
    //ScratchComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    /***** Equip *****/
    // Weapon�� ������ ���� �̸�
    WeaponSocketName = "Scratch_L";
}


/***** Attack *****/
void AC_EWeaponScratch::OnEWeaponScratchOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 1. �浹�� character�� player���� Ȯ���Ѵ�
    AC_PlayerCharacter* player = Cast<AC_PlayerCharacter>(OtherActor);
    CheckNull(player);

    // 2. player���� �������� ������
    player->PlayerHit(Owner->GetMeleeDamage());
    //*** Debug
    UE_LOG(LogTemp, Warning, TEXT(">>> Enemy Attack Succeeded!"));
}

USphereComponent* AC_EWeaponScratch::GetScratchComp()
{
    return ScratchComp;
}

