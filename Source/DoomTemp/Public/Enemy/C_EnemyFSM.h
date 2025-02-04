// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
    SPAWN,			// ����
    IDLE,			// ���
    PATROL,			// ����
    MOVE,			// �̵�
    ATTACK,			// ����
	DAMAGE,         // �ǰ�
	DEAD,			// ����
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
	// ���� ����
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FSM")
	EEnemyState EnemyState = EEnemyState::IDLE;

	FVector Direction;	// Enemy �̵� ����

    UPROPERTY(EditDefaultsOnly, Category = "FSM")
	float IdleDelayTime = 3.f;		// ��� �ð�
	float CurTime = 0.f;			// ��� �ð�
	UPROPERTY(EditDefaultsOnly, Category = "FSM")
	float AttackDelayTime = 2.f;


	/***** Target & Self *****/
	UPROPERTY(VisibleAnywhere, Category = "FSM")
	class AC_PlayerCharacter* Target;
	UPROPERTY()
	class AC_EnemyA* Self;


	/***** Attack Range *****/
	float MeleeRange;	// �ٰŸ� ���� ����
	float LongRange;	// ��Ÿ� ���� ����

public:
	// ����
	void SpawnState();

	// ���
	void IdleState();

	// ����
	void PatrolState();

	// �̵�
	void MoveState();

	// ����
	void AttackState();

	// �ǰ�
	void DamageState();

	// ����
	void DeadState();
};