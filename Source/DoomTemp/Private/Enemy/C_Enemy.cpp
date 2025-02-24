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
    // 월드에 배치되거나 스폰될 때 자동으로 AIController로부터 Possess되도록 설정
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

    // Spawn 시 player를 바라보도록
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

	/***** Enemy 상태 체크 *****/
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


/***** Enemy 상태 체크 *****/
void AC_Enemy::CheckSubState()
{
    /* Enemy Move : Walk > Flinch > Stagger > Dead */

    if (HP > HPFlinched)
    {
        // 이동 중에 체크했다면
        if(FSM->GetEnemyState() == EEnemyState::MOVE)
            FSM->SetEnemyMovement(EEnemyMovement::WALK);
        // 정지 상태에서 체크했다면
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

    // 애니메이션 상태 동기화
    FSM->SetAnimState(FSM->GetEnemyState());
    FSM->SetAnimSubStateMovement(FSM->GetEnemyMovement());

    // Speed 조절
    SetEnemySpeed();

    // 공격력 조절
    ChangeMeleeDamage();
}


/***** 데미지 처리 *****/
void AC_Enemy::OnDamageProcess(int32 InDamage, enum class EAttackType InAttackType)
{
    // HP를 깎는다
    SetHP(InDamage);

    // Enemy 상태 변경
    FSM->SetEnemyState(EEnemyState::DAMAGE);
    CheckSubState();

    // 길찾기 기능 정지
    FSM->GetMyAI()->StopMovement();

    // Attack Type에 따른 애니메이션 및 이펙트 실행
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


/***** 사망 처리 *****/
void AC_Enemy::OnDead()
{
    GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Cyan, FString("Enemy Dead !!!!!"));

    // 죽음 애니메이션이 재생된다
    SectionName = FString::Printf(TEXT("Dead"));
    FSM->PlayEnemyMontage(&SectionName);

    // 바닥에 피 VFX가 나타난다

    // 들고 있던 Weapon을 Destroy한다
    WeaponComps->WeaponDestroy();
}


// Enemy 상태에 따른 Speed 변경
void AC_Enemy::SetEnemySpeed()
{
    EEnemyState state = FSM->GetEnemyState();
    EEnemyMovement movement = FSM->GetEnemyMovement();

    // 1. Enemy 상태에 따른 speed 변경
    if (state == EEnemyState::MOVE && movement == EEnemyMovement::WALK)
        SetSpeed(SpeedMax);
    else
        SetSpeed(0);   // ATTACK, DAMAGE, FLINCH, STAGGER, DEAD

    // 2. 변경 내용 적용
    GetCharacterMovement()->MaxWalkSpeed = Speed;
}


// Enemy 상태에 따른 Melee Attack Damage 변경
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

    // 1. FLINCH, STAGGER 상태가 아닐 때만 맞는 애니메이션 재생
    SectionName = FString::Printf(TEXT("Damage%d"), 0);
    FSM->PlayDamageAM(&SectionName);

    // 2. 맞은 위치에 피 튀기는 VFX 나올 컴포넌트 부착

    // 3. 부착한 컴포넌트에서 맞은 부분의 normal 방향으로 피 튀기는 VFX 소환

}


void AC_Enemy::OnDamageGun()
{
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Get Damage GUN-----");

    // 1. FLINCH, STAGGER 상태가 아닐 때만 맞는 애니메이션 재생
    SectionName = FString::Printf(TEXT("Damage%d"), 0);
    FSM->PlayDamageAM(&SectionName);

    // 2. 맞은 위치에 피 튀기는 VFX 나올 컴포넌트 부착

    // 3. 부착한 컴포넌트에서 맞은 부분의 normal 방향으로 피 튀기는 VFX 소환

}


void AC_Enemy::OnDamageGloryKill()
{
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Get Damage GLORYKILL-----");

    // 1. FLINCH, STAGGER 상태가 아닐 때만 맞는 애니메이션 재생
    SectionName = FString::Printf(TEXT("Damage%d"), 0);
    FSM->PlayDamageAM(&SectionName);

    // 2. 맞은 위치에 피 뿜어져 나오는 VFX 나올 컴포넌트 부착

    // 3. 부착한 컴포넌트에서 맞은 부분의 normal 방향으로 피 뿜어져 나오는 VFX 소환

}


void AC_Enemy::OnDamageChainsaw()
{
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Get Damage CHAINSAW-----");

    // 1. FLINCH, STAGGER 상태가 아닐 때만 맞는 애니메이션 재생
    SectionName = FString::Printf(TEXT("Damage%d"), 0);
    FSM->PlayDamageAM(&SectionName);

    // 2. 맞은 위치에 피 뿜어져 나오는 VFX 나올 컴포넌트 부착

    // 3. 부착한 컴포넌트에서 맞은 부분의 normal 방향으로 피 뿜어져 나오는 VFX 소환

}