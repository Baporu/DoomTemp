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


    ///***** Register Niagara Component for Electric Force *****/
    //ElectricForceComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ElectricForceNiagara"));
    //ElectricForceComp->SetupAttachment(RootComponent);
    //ElectricForceComp->SetAutoActivate(false);
    //ConstructorHelpers::FObjectFinder<UNiagaraSystem> ElectricForceVFX(TEXT("NiagaraSystem'/Game/Designs/Knife_light/VFX/NE_attack03.NE_attack03'"));
    //if (ElectricForceVFX.Succeeded())
    //{
    //    ElectricForceComp->SetAsset(ElectricForceVFX.Object);
    //    ElectricForceComp->SetRelativeLocation(GetActorLocation() + FVector(0.f, 0.f, 25));
    //    ElectricForceComp->SetRelativeScale3D(FVector(0.6f));
    //}
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
    else if (HP > HPStaggered)
        FSM->SetEnemyMovement(EEnemyMovement::FLINCH);
    else if (HP > 0.f)
        FSM->SetEnemyMovement(EEnemyMovement::STAGGER);
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
void AC_Enemy::OnDamageProcess(int32 InDamage, enum class EAttackType InAttackType)
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


/***** ��� ó�� *****/
void AC_Enemy::OnDead()
{
    GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Cyan, FString("Enemy Dead !!!!!"));

    // ���� �ִϸ��̼��� ����ȴ�
    SectionName = FString::Printf(TEXT("Dead"));
    FSM->PlayEnemyMontage(&SectionName);

    // �ٴڿ� �� VFX�� ��Ÿ����

    // ��� �ִ� Weapon�� Destroy�Ѵ�
    WeaponComps->WeaponDestroy();
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

    // 1. FLINCH, STAGGER ���°� �ƴ� ���� �´� �ִϸ��̼� ���
    SectionName = FString::Printf(TEXT("Damage%d"), 0);
    FSM->PlayDamageAM(&SectionName);

    // 2. ���� ��ġ�� �� Ƣ��� VFX ���� ������Ʈ ����

    // 3. ������ ������Ʈ���� ���� �κ��� normal �������� �� Ƣ��� VFX ��ȯ

}


void AC_Enemy::OnDamageGun()
{
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Get Damage GUN-----");

    // 1. FLINCH, STAGGER ���°� �ƴ� ���� �´� �ִϸ��̼� ���
    SectionName = FString::Printf(TEXT("Damage%d"), 0);
    FSM->PlayDamageAM(&SectionName);

    // 2. ���� ��ġ�� �� Ƣ��� VFX ���� ������Ʈ ����

    // 3. ������ ������Ʈ���� ���� �κ��� normal �������� �� Ƣ��� VFX ��ȯ

}


void AC_Enemy::OnDamageGloryKill()
{
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Get Damage GLORYKILL-----");

    // 1. FLINCH, STAGGER ���°� �ƴ� ���� �´� �ִϸ��̼� ���
    SectionName = FString::Printf(TEXT("Damage%d"), 0);
    FSM->PlayDamageAM(&SectionName);

    // 2. ���� ��ġ�� �� �վ��� ������ VFX ���� ������Ʈ ����

    // 3. ������ ������Ʈ���� ���� �κ��� normal �������� �� �վ��� ������ VFX ��ȯ

}


void AC_Enemy::OnDamageChainsaw()
{
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Get Damage CHAINSAW-----");

    // 1. FLINCH, STAGGER ���°� �ƴ� ���� �´� �ִϸ��̼� ���
    SectionName = FString::Printf(TEXT("Damage%d"), 0);
    FSM->PlayDamageAM(&SectionName);

    // 2. ���� ��ġ�� �� �վ��� ������ VFX ���� ������Ʈ ����

    // 3. ������ ������Ʈ���� ���� �κ��� normal �������� �� �վ��� ������ VFX ��ȯ

}