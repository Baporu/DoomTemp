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


/***** Sub State - Movement Type *****/
// TickComponent���� ���� X - Enemy�� Tick���� üũ��
UENUM(BlueprintType)
enum class EEnemyMovement : uint8
{
	WALK = 0,			// �ȱ�
	FLINCH,				// ��Ʋ�Ÿ���
	STAGGER,			// ����Ÿ���
	MAX
};

/***** Sub State - Attack Type *****/
// Player���� ������ / ���� ����
enum class EAttackType;

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

	// Sub state - ��� �����̰� �ִ°�
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FSM")
	EEnemyMovement EnemyMovement = EEnemyMovement::MAX;


	// Sub state - � ������ �޾Ҵ°�
	EAttackType AttackType;
	

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
    UPROPERTY(EditAnywhere, Category = "FSM")
	float DamageDelayTime = 2.f;	// �ǰ� ��� �ð�
	UPROPERTY(EditAnywhere, Category = "FSM")
	float DestroyDelayTime = 3.f;	// ���� �� ����� ��� �ð�


	/***** Target & Self *****/
	UPROPERTY(VisibleAnywhere, Category = "FSM")
	class AC_PlayerCharacter* Target;
	UPROPERTY()
	class AC_EnemyA* Self;


	/***** Attack Range *****/
	float MeleeRange;	// �ٰŸ� ���� ����
	//float LongRange;	// ��Ÿ� ���� ����


	/***** Main State *****/
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


	/***** Sub State *****/
public:
	// �ȱ�
	void WalkMovement();

	// ��Ʋ�Ÿ� (Flinch)
	void FlinchMovement();

	// ����Ÿ� (Stager)
	void StaggerMovement();


	/***** Getters *****/
public:
	EEnemyMovement GetEnemyMovement();


	/***** Setters *****/
public:
	void SetEnemyMovement(EEnemyMovement InVal);
	void SetEnemyState(EEnemyState InVal);
	void SetAttackType(EAttackType InVal);


public:
	// �̵� �� ����
	void Move();
	// �Ÿ��� ���� �ٰŸ� ���� ���·� ��ȯ
	void CheckDistance(float InDistance);
};