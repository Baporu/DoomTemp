// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_EnemyFSM.generated.h"

/***** Main State *****/
// TickComponent에서 관리
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
    IDLE = 0,			// 대기
    MOVE,				// 이동
    ATTACK,				// 공격
	DAMAGE,				// 피격
	DEAD,				// 죽음
	MAX					// 등장
};


/***** Sub State - Movement *****/
// TickComponent에서 관리 X - Enemy쪽 Tick에서 체크함
UENUM(BlueprintType)
enum class EEnemyMovement : uint8
{
	WALK = 0,			// 걷기
	FLINCH,				// 비틀거리기
	STAGGER,			// 주춤거리기
	DEAD	,			// 죽음
	MAX
};


/***** Sub State - Damaged *****/
// TickComponent에서 관리 X - Enemy쪽 Tick에서 체크함
UENUM(BlueprintType)
enum class EEnemyDamaged : uint8
{
    FIST = 0,			// 주먹
    GUN,				// 총
    GLORYKILL,			// 글로리 킬
    CHAINSAW,			// 전기톱
    MAX					// 공격받고 있지 않음
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOOMTEMP_API UC_EnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	UC_EnemyFSM();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	/***** State *****/
	// Main state
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FSM")
	EEnemyState EnemyState = EEnemyState::MAX;

	// Sub state - 어떤 공격을 받았는가
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FSM")
	EEnemyDamaged EnemyDamaged = EEnemyDamaged::MAX;

	// Sub state - 어떻게 움직이고 있는가
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FSM")
	EEnemyMovement EnemyMovement = EEnemyMovement::WALK;
	

	/***** AI Controller *****/
	// Enemy를 소유하고 있는 AIController
	UPROPERTY()
	class AAIController* MyAI;
	

	/***** Patrol *****/
	FVector RandPos;	// 길 찾기 수행 시 랜덤 위치
	bool GetRandomPositionInNavMesh(FVector InCenterLocation, float InRadius, FVector& InDest);


	/***** Time *****/
    UPROPERTY(EditDefaultsOnly, Category = "FSM")
	float IdleDelayTime = 3.f;		// 대기 시간
	float CurTime = 0.f;			// 경과 시간
	UPROPERTY(EditDefaultsOnly, Category = "FSM")
	float AttackDelayTime = 2.f;	// 공격 대기 시간
    UPROPERTY(EditAnywhere, Category = "FSM")
	float DamageDelayTime = 2.f;	// 피격 대기 시간
	UPROPERTY(EditAnywhere, Category = "FSM")
	float DestroyDelayTime = 3.f;	// 죽음 후 사라짐 대기 시간


	/***** Target & Self *****/
	UPROPERTY(VisibleAnywhere, Category = "FSM")
	class AC_PlayerCharacter* Target;
	UPROPERTY()
	class AC_EnemyA* Self;


	/***** Attack Range *****/
	float MeleeRange;	// 근거리 공격 범위
	//float LongRange;	// 장거리 공격 범위


public:
	// 등장
	void SpawnState();

	// 대기
	void IdleState();

	// 이동
	void MoveState();

	// 공격
	void AttackState();

	// 피격
	void DamageState();
	void OnDamageProcess(int32 InVal);

	// 죽음
	void DeadState();

public:
	void SetEnemyDamaged(EEnemyDamaged InVal);
	void SetEnemyMovement(EEnemyMovement InVal);
	void SetEnemyState(EEnemyState InVal);
};