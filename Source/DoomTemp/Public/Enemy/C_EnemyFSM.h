// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_EnemyFSM.generated.h"

/***** Main State *****/
// TickComponent���� ����
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
    IDLE = 0,			// ���
    MOVE,				// �̵�
    ATTACK,				// ����
	DAMAGE,				// �ǰ�
	DEAD,				// ����
	MAX					// ����
};


/***** Sub State - Damaged *****/
// TickComponent���� ���� X - Enemy�� Tick���� üũ��
UENUM(BlueprintType)
enum class EEnemyDamaged : uint8
{
	FIST = 0,			// �ָ�
	GUN,				// ��
	GLORYKILL,			// �۷θ� ų
	CHAINSAW,			// ������
	MAX					// ���ݹް� ���� ����
};


/***** Sub State - Move *****/
// TickComponent���� ���� X - Enemy�� Tick���� üũ��
UENUM(BlueprintType)
enum class EEnemyMove : uint8
{
	STAGGER = 0,	// ����Ÿ���
	FLINCH,			// ��Ʋ�Ÿ���
	WALK,			// �ȱ�
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

protected:
	/***** State *****/
	// Main state
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FSM")
	EEnemyState EnemyState = EEnemyState::MAX;

	// Sub state - � ������ �޾Ҵ°�
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FSM")
	EEnemyDamaged EnemyDamaged = EEnemyDamaged::MAX;

	// Sub state - ��� �����̰� �ִ°�
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FSM")
	EEnemyMove EnemyMove = EEnemyMove::WALK;
	

	/***** AI Controller *****/
	// Enemy�� �����ϰ� �ִ� AIController
	UPROPERTY()
	class AAIController* MyAI;
	

	/***** Patrol *****/
	FVector RandPos;	// �� ã�� ���� �� ���� ��ġ
	bool GetRandomPositionInNavMesh(FVector InCenterLocation, float InRadius, FVector& InDest);


	/***** Time *****/
    UPROPERTY(EditDefaultsOnly, Category = "FSM")
	float IdleDelayTime = 3.f;		// ��� �ð�
	float CurTime = 0.f;			// ��� �ð�
	UPROPERTY(EditDefaultsOnly, Category = "FSM")
	float AttackDelayTime = 2.f;	// ���� ��� �ð�


	/***** Target & Self *****/
	UPROPERTY(VisibleAnywhere, Category = "FSM")
	class AC_PlayerCharacter* Target;
	UPROPERTY()
	class AC_EnemyA* Self;


	/***** Attack Range *****/
	float MeleeRange;	// �ٰŸ� ���� ����
	//float LongRange;	// ��Ÿ� ���� ����


public:
	// ����
	void SpawnState();

	// ���
	void IdleState();

	// �̵�
	void MoveState();

	// ����
	void AttackState();

	// �ǰ�
	void DamageState();

	// ����
	void DeadState();

public:
	EEnemyMove GetEnemyMove();
	void SetEnemyDamaged(EEnemyDamaged InVal);
	void SetEnemyMove(EEnemyMove InVal);
};