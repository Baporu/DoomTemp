#include "Enemy/C_Enemy.h"
#include "C_Helpers.h"
#include "GameFramework/Character.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Enemy/C_EnemyFSM.h"
#include "C_PlayerCharacter.h"
#include "Enemy/C_EWeaponComp.h"
#include "Enemy/C_EnemyAAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "DropBase.h"

/* headers for VFX */
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h"


AC_Enemy::AC_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

    /***** Speed *****/
    GetCharacterMovement()->MaxWalkSpeed = Speed;

	/***** FSM *****/
    C_Helpers::CreateActorComponent<UC_EnemyFSM>(this, &FSM, "FSM");

    /***** Weapon *****/
    C_Helpers::CreateActorComponent<UC_EWeaponComp>(this, &WeaponComps, "Weapon");

    /***** AI Controller *****/
    // ���忡 ��ġ�ǰų� ������ �� �ڵ����� AIController�κ��� Possess�ǵ��� ����
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


    /***** Niagara *****/
    // Fist
    C_Helpers::CreateComponent(this, &NiagaraCompFist, FName("NiagaraCompFist"), GetCapsuleComponent());
    NiagaraCompFist->SetAutoActivate(false);

    C_Helpers::GetAsset(&NiagaraSysFist, FString("/Script/Niagara.NiagaraSystem'/Game/sA_BloodSplatter_System/Fx/NS_Splatter_Hit_6.NS_Splatter_Hit_6'"));


    // Gun
    C_Helpers::CreateComponent(this, &NiagaraCompGun, FName("NiagaraCompGun"), GetCapsuleComponent());
    NiagaraCompGun->SetAutoActivate(false);

    C_Helpers::GetAsset(&NiagaraSysGun, FString("/Script/Niagara.NiagaraSystem'/Game/sA_BloodSplatter_System/Fx/NS_Splatter_Hit_2.NS_Splatter_Hit_2'"));


    // Glory Kill
    C_Helpers::CreateComponent(this, &NiagaraCompGloryKill, FName("NiagaraCompGloryKill"), GetCapsuleComponent());
    NiagaraCompGloryKill->SetAutoActivate(false);

    C_Helpers::GetAsset(&NiagaraSysGloryKill, FString("/Script/Niagara.NiagaraSystem'/Game/sA_BloodSplatter_System/Fx/NS_Splatter_Hit_4.NS_Splatter_Hit_4'"));


    // Chainsaw
    C_Helpers::CreateComponent(this, &NiagaraCompChainsaw, FName("NiagaraCompChainsaw"), GetCapsuleComponent());
    NiagaraCompChainsaw->SetAutoActivate(false);

    C_Helpers::GetAsset(&NiagaraSysChainsaw, FString("/Script/Niagara.NiagaraSystem'/Game/sA_BloodSplatter_System/Fx/NS_Splatter_Slash_3.NS_Splatter_Slash_3'"));
}


void AC_Enemy::BeginPlay()
{
	Super::BeginPlay();

    // Spawn �� player�� �ٶ󺸵���
    APawn* player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (player)
    {
        //ChangeRotation();
        FRotator dir = (player->GetActorLocation() - GetActorLocation()).Rotation();
        SetActorRotation(FRotator(0.f, dir.Yaw, 0.f));
    }
}


void AC_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/***** Enemy ���� üũ *****/
    //CheckSubState();
}


/***** Getters *****/
float AC_Enemy::GetMeleeRange() { return MeleeRange; }

int32 AC_Enemy::GetMeleeDamage() { return MeleeDamage; }

//float AC_Enemy::GetLongRange() { return LongRange; }

float AC_Enemy::GetSpeed() { return Speed; }

UC_EnemyFSM* AC_Enemy::GetEnemyFSM() { return FSM; }

UC_EWeaponComp* AC_Enemy::GetWeaponComps() { return WeaponComps; }


/***** Setters *****/
void AC_Enemy::SetHP(int32 InVal) { HP -= InVal; }

void AC_Enemy::SetMeleeDamage(int32 InVal) { MeleeDamage = InVal; }

void AC_Enemy::SetSpeed(float InVal) { Speed = InVal; }


/***** Enemy ���� üũ *****/
void AC_Enemy::CheckSubState()
{
    /* Enemy Move : Walk > Flinch > Stagger > Dead */

    if (HP > HPFlinched)
    {
        // �̵� �߿� üũ�ߴٸ�
        if(FSM->GetEnemyState() == EEnemyState::MOVE)
            FSM->SetEnemyMovement(EEnemyMovement::WALK);
        // ���� ���¿��� üũ�ߴٸ�
        if (FSM->GetEnemyState() == EEnemyState::IDLE)
            FSM->SetEnemyMovement(EEnemyMovement::MAX);
    }
    else if (HP > HPStaggered) {
        bIsFlinched = true;

        FSM->SetEnemyMovement(EEnemyMovement::FLINCH);
    }
    else if (HP > 0.f) {
        bIsStaggered = true;

        FSM->SetEnemyMovement(EEnemyMovement::STAGGER);
    }
    else
    {
        //FSM->SetEnemyState(EEnemyState::DEAD);
        FSM->ChangeEnemyStates(EEnemyState::DEAD, EEnemyMovement::MAX);
        OnDead();
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    // �ִϸ��̼� ���� ����ȭ
    FSM->SetAnimState(FSM->GetEnemyState());
    FSM->SetAnimSubStateMovement(FSM->GetEnemyMovement());

    // Speed ����
    SetEnemySpeed();

    // ���ݷ� ����
    ChangeMeleeDamage();
}


/***** ������ ó�� *****/
void AC_Enemy::OnDamageProcess(int32 InDamage, EAttackType InAttackType)
{
    // HP�� ��´�
    SetHP(InDamage);

    // Enemy ���� ����
    FSM->SetEnemyState(EEnemyState::DAMAGE);
    CheckSubState();

    // ��ã�� ��� ����
    FSM->GetMyAI()->StopMovement();

    // Attack Type�� ���� �ִϸ��̼� �� ����Ʈ ����
    switch (InAttackType)
    {
    case EAttackType::Fist:
        OnDamageFist();
        break;
    case EAttackType::Gun:
        OnDamageGun();
        break;
    case EAttackType::GloryKill:
        OnDamageGloryKill();
        break;
    case EAttackType::Chainsaw:
        OnDamageChainsaw();
        break;
    }
}


void AC_Enemy::OnDamageProcess(int32 InDamage, enum class EAttackType InAttackType, FVector InHitPos, FVector InHitPointNormal)
{
    // HP�� ��´�
    SetHP(InDamage);

    // Hit�� �߻��� ���� ������ �޴´�
    HitPos = InHitPos;
    HitPointDir = FRotator(InHitPointNormal.Y, InHitPointNormal.Z, InHitPointNormal.X);

    // Enemy ���� ����
    FSM->SetEnemyState(EEnemyState::DAMAGE);
    CheckSubState();

    // ��ã�� ��� ����
    FSM->GetMyAI()->StopMovement();

    // Attack Type�� ���� �ִϸ��̼� �� ����Ʈ ����
    switch (InAttackType)
    {
    case EAttackType::Fist:
        OnDamageFist();
        break;
    case EAttackType::Gun:
        OnDamageGun();
        break;
    case EAttackType::GloryKill:
        OnDamageGloryKill();
        break;
    case EAttackType::Chainsaw:
        OnDamageChainsaw();
        break;
    }
}


/***** ��� ó�� *****/
void AC_Enemy::OnDead()
{
    GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Cyan, FString("Enemy Dead !!!!!"));

    // ���� �ִϸ��̼��� ����ȴ�
    SectionName = FString::Printf(TEXT("Dead"));
    FSM->PlayEnemyMontage(&SectionName);

    // �� VFX�� ��Ÿ����

    // ��� �ִ� Weapon�� Destroy�Ѵ�
    WeaponComps->WeaponDestroy();

    AC_PlayerCharacter* player = Cast<AC_PlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

    if (player)
        player->CheckGameEnd();
}


// Enemy ���¿� ���� Speed ����
void AC_Enemy::SetEnemySpeed()
{
    EEnemyState state = FSM->GetEnemyState();
    EEnemyMovement movement = FSM->GetEnemyMovement();

    // 1. Enemy ���¿� ���� speed ����
    if (state == EEnemyState::MOVE && movement == EEnemyMovement::WALK)
        SetSpeed(SpeedMax);
    else
        SetSpeed(0);   // ATTACK, DAMAGE, FLINCH, STAGGER, DEAD

    // 2. ���� ���� ����
    GetCharacterMovement()->MaxWalkSpeed = Speed;
}


// Enemy ���¿� ���� Melee Attack Damage ����
void AC_Enemy::ChangeMeleeDamage()
{
    auto movement = FSM->GetEnemyMovement();

    if (movement == EEnemyMovement::WALK)
        SetMeleeDamage(MeleeDamageMax);
    else if (movement == EEnemyMovement::FLINCH)
        SetMeleeDamage(MeleeDamageFlinch);
    else
        SetMeleeDamage(0);   // STAGGER, DEAD
}

//void AC_Enemy::ChangeRotation()
//{
//    FRotator dir = (player->GetActorLocation() - GetActorLocation()).Rotation();
//    SetActorRotation(FRotator(0.f, dir.Yaw, 0.f));
//}


/***** Damage Events *****/
void AC_Enemy::OnDamageFist()
{
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Get Damage FIST-----");

    // FLINCH, STAGGER ���°� �ƴ� ���� �´� �ִϸ��̼� ���
    SectionName = FString::Printf(TEXT("Damage%d"), 0);
    FSM->PlayDamageAM(&SectionName);

    // ���� ��ġ�� �� Ƣ��� VFX Spawn

}


void AC_Enemy::OnDamageGun()
{
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Get Damage GUN-----");

    // FLINCH, STAGGER ���°� �ƴ� ���� �´� �ִϸ��̼� ���
    SectionName = FString::Printf(TEXT("Damage%d"), 0);
    FSM->PlayDamageAM(&SectionName);

    // ���� ��ġ�� �� Ƣ��� VFX Spawn
    SpawnNiagara(NiagaraCompGun, NiagaraSysGun);

    //UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, NiagaraSysFist, GetActorLocation(), GetActorRotation(), FVector(1.2f));
}


void AC_Enemy::OnDamageGloryKill()
{
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Get Damage GLORYKILL-----");

    // 1. FLINCH, STAGGER ���°� �ƴ� ���� �´� �ִϸ��̼� ���
    SectionName = FString::Printf(TEXT("Damage%d"), 0);
    FSM->PlayDamageAM(&SectionName);

    // ���� ��ġ�� �� Ƣ��� VFX Spawn
    SpawnNiagara(NiagaraCompGloryKill, NiagaraSysGloryKill);

}


void AC_Enemy::OnDamageChainsaw()
{
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Get Damage CHAINSAW-----");

    // FLINCH, STAGGER ���°� �ƴ� ���� �´� �ִϸ��̼� ���
    SectionName = FString::Printf(TEXT("Damage%d"), 0);
    FSM->PlayDamageAM(&SectionName);

    // ���� ��ġ�� �� Ƣ��� VFX Spawn
    SpawnNiagara(NiagaraCompChainsaw, NiagaraSysChainsaw);

    SpawnDrops();
}

void AC_Enemy::SpawnDrops()
{
    FActorSpawnParameters params;
    params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    for (int32 i = 0; i < DropTypes.Num(); i++) {
        int32 randCount = FMath::RandRange(0, DropCounts[i]);

        for (int32 j = 0; j < randCount; j++) {
            GetWorld()->SpawnActor<ADropBase>(DropTypes[i], GetActorLocation(), FRotator::ZeroRotator, params);
        }
    }
}


// Spawn Niagara
void AC_Enemy::SpawnNiagara(UNiagaraComponent* InComp, UNiagaraSystem* InSys, FVector InScale)
{
    InComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), InSys, HitPos, HitPointDir, InScale);
    InComp->Activate();
}


// Deactivate Niagara System
void AC_Enemy::DeActivateNiagara(UNiagaraComponent* InComp)
{
    InComp->Deactivate();
}