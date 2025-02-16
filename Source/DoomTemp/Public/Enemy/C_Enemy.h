#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_Enemy.generated.h"

// abstract : Unreal Editor���� �߻� ���̽� Ŭ������ ����Ͽ� Map�� ��ġ�� �� ����
UCLASS(abstract)
class DOOMTEMP_API AC_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	AC_Enemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	/***** Enemy Information *****/
	#pragma region Enemy Information
    // Default �� : EnemyA�� ��������
	
	int32 HPMax = 200;
	int32 HP = HPMax;

	float SpeedMax = 180.f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Info")
    float Speed = SpeedMax;
	float SpeedFlinch = SpeedMax * 0.7;

	int32 HPFlinched = HP * 0.5;		// ��Ʋ�Ÿ��� ����
	int32 HPStaggered = HP * 0.33;	// ����Ÿ��� ����

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Info")
	float MeleeRange = 80.f;			// �ٰŸ� �ν� �Ÿ�

	int32 MeleeDamageMax = 15;
	int32 MeleeDamage = MeleeDamageMax;				// �ٰŸ� ���� �� ���� ������
	int32 MeleeDamageFlinch = MeleeDamage * 0.6;	// Flinch ���¿��� ���� �ٰŸ� ���� ������


	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy|Info")
	//float LongRange = 0.f;				// ���Ÿ� �ν� �Ÿ�
	//int32 LongRangeDamage = 0;			// ���Ÿ� ���� �� ���� ������


	/***** FSM *****/
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FSMComp")
	class UC_EnemyFSM* FSM;


	/***** Weapon *****/
	// Weapon component �ȿ� Weapon�� �����ϱ� Weapon Comp�� Weapon�� ���� ����
    UPROPERTY(VisibleAnywhere)
	class UC_EWeaponComp* Weapon;
	#pragma endregion


public:
	bool bIsFlinched = false;
	bool bIsStaggered = false;
	bool bIsDead = false;


	/***** Getters *****/
public:
	float GetMeleeRange();
	//float GetLongRange();

	int32 GetMeleeDamage();
	float GetSpeed();
	
	
	/***** Setters *****/
protected:
	void SetHP(int32 InVal);
	void SetMeleeDamage(int32 InVal);
	void SetSpeed(float InVal);


	/***** Functions *****/
public:
	void CheckState();		// ����Ÿ��� ��������, ��Ʋ�Ÿ��� ��������, �׾����� üũ
	void OnDamaged(int32 InVal, enum class EAttackType InAttackType);
	void OnDead();			// Enemy ��� �� ó���� ��

	void ChangeSpeed();			// Enemy ���¿� ���� Speed ����
	void ChangeMeleeDamage();	// Enemy ���¿� ���� Melee Attack Damage ����
};
