// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
    SPAWN,			// 등장
    IDLE,			// 대기
    PATROL,			// 순찰
    MOVE,			// 이동
    ATTACK,			// 공격
	DAMAGE,         // 피격
	DEAD,			// 죽음
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

	FVector Direction;	// Enemy 이동 방향

    UPROPERTY(EditDefaultsOnly, Category = "FSM")
	float IdleDelayTime = 3.f;		// 대기 시간
	float CurTime = 0.f;			// 경과 시간
	UPROPERTY(EditDefaultsOnly, Category = "FSM")
	float AttackDelayTime = 2.f;


	/***** Target & Self *****/
	UPROPERTY(VisibleAnywhere, Category = "FSM")
	class AC_PlayerCharacter* Target;
	UPROPERTY()
	class AC_EnemyA* Self;


	/***** Attack Range *****/
	float MeleeRange;	// 근거리 공격 범위
	float LongRange;	// 장거리 공격 범위

public:
	// 등장
	void SpawnState();

	// 대기
	void IdleState();

	// 순찰
	void PatrolState();

	// 이동
	void MoveState();

	// 공격
	void AttackState();

	// 피격
	void DamageState();

	// 죽음
	void DeadState();
};