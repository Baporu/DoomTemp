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
	SPAWN,				// ����
	MAX					
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
	EEnemyState EnemyState = EEnemyState::IDLE;

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

	UPROPERTY(EditAnywhere, Category = "FSM")
	float CurTime = 0.f;			// ��� �ð�

	UPROPERTY(EditAnywhere, Category = "FSM")
	float AttackDelayTime = 0.7f;	// ���� ��� �ð�

    UPROPERTY(EditAnywhere, Category = "FSM")
	float DamageDelayTime = 3.f;	// �ǰ� ��� �ð�

	UPROPERTY(EditAnywhere, Category = "FSM")
	float DestroyDelayTime = 4.5f;	// ���� �� ����� ��� �ð�


	/***** Target & Self *****/
	UPROPERTY(VisibleAnywhere, Category = "FSM")
	class AC_PlayerCharacter* Target;
	UPROPERTY()
	class AC_EnemyA* Self;


	/***** Attack Range *****/
	float MeleeRange;	// �ٰŸ� ���� ����
	//float LongRange;	// ��Ÿ� ���� ����


	/***** Animation *****/
	UPROPERTY()
	class UC_EnemyAAnimInstance* Anim;


	/***** Dead *****/
	bool bDeadDone;


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
	void OnAttackEnd();	// Attack Notify �����

	// �ǰ�
	void DamageState();

	// ����
	void DeadState();
	void onDeadEnd();	// DeadEnd Notify �����


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
	EEnemyState GetEnemyState();
	AAIController* GetMyAI();

	/***** Setters *****/
public:
	void SetEnemyMovement(EEnemyMovement InVal);
	void SetEnemyState(EEnemyState InVal);
	void SetAttackType(EAttackType InVal);
	void SetAnimState(EEnemyState InVal);
	void SetAnimSubStateMovement(EEnemyMovement InVal);


public:
	// �̵� �� ����
	void Move();

	// �Ÿ��� ���� �ٰŸ� ���� ���·� ��ȯ
	void CanMeleeAttack(float InDistance);

	// Animation Montage ���
	void PlayEnemyMontage(FString* InSectionName);

	// �ٰŸ� ����� MOVE�� ��ȯ
	void CanMove(float InDistance);

	// Enemy Main state �� Movement state ����
	void ChangeEnemyStates(EEnemyState InMain, EEnemyMovement InMovement);

	// Enemy Movement ���¿� ���� Damage Animation Montage ���
	void PlayDamageAM(FString* InSectionName);
};