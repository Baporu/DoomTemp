#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_Enemy.generated.h"

UCLASS()
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
	
	int32 MaxHP = 200;
	int32 HP = MaxHP;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Info")
	float Speed = 180.f;

	int32 HPFlinched = HP * 0.5;		// ��Ʋ�Ÿ��� ����
	int32 HPStaggered = HP * 0.33;	// ����Ÿ��� ����

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Info")
	float MeleeRange = 100.f;			// �ٰŸ� �ν� �Ÿ�
	int32 MeleeDamage = 15;				// �ٰŸ� ���� �� ���� ������

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy|Info")
	//float LongRange = 0.f;				// ���Ÿ� �ν� �Ÿ�
	//int32 LongRangeDamage = 0;			// ���Ÿ� ���� �� ���� ������


	/***** FSM *****/
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FSMComp")
	class UC_EnemyFSM* FSM;


	/***** Weapon *****/
    UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class AC_EWeapon* Weapon;
	#pragma endregion


public:
	bool bIsFlinched = false;
	bool bIsStaggered = false;
	bool bIsDead = false;

public:
	/***** Getters *****/
	float GetMeleeRange();
	//float GetLongRange();
	float GetSpeed();


	/***** Setters *****/
	void SetHP(int32 InHP);


	/***** Functions *****/
	void CheckState();		// ����Ÿ��� ��������, ��Ʋ�Ÿ��� ��������, �׾����� üũ
	void OnDamaged(int32 InDamage, enum class EAttackType );
	void OnDead();			// Enemy ��� �� ó���� ��
};
