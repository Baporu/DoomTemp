// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_EnemyFSM.generated.h"

UENUM(BlueprintType)
/***** Main State *****/
enum class EEnemyState : uint8
{
    SPAWN,			// 등장
    IDLE,			// 대기
    MOVE,			// 이동
    ATTACK,			// 공격
	DAMAGE,         // 피격
	DEAD,			// 죽음
	MAX
};


/***** Sub State - Damaged *****/
enum class EEnemyDamaged : uint8
{
	FIST, 
	GUN, 
	GLORYKILL, 
	CHAINSAW,
	MAX
};


/***** Sub State - Move *****/
enum class EEnemyMove
{
	WALK, 
	RUN, 
	STAGGER, 
	MAX
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

public:
	// 상태 변수
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FSM")
	EEnemyState EnemyState = EEnemyState::IDLE;
	
	// Enemy를 소유하고 고 있는 AIController
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

	// 죽음
	void DeadState();
};