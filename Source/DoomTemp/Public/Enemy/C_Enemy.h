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

	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    //////////////////////////////////////////// Enemy Information //////////////////////////////////////////// 
    // Default �� : EnemyA�� ��������
	
	int32 MaxHP = 200;
	int32 HP = MaxHP;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy|Info")
	float Speed = 180.f;

	int32 HPFlinched = HP * 0.5;		// ��Ʋ�Ÿ��� ����
	int32 HPStaggered = HP * 0.33;	// ����Ÿ��� ����

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy|Info")
	float MeleeRange = 100.f;			// �ٰŸ� �ν� �Ÿ�
	int32 MeleeDamage = 15;				// �ٰŸ� ���� �� ���� ������

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy|Info")
	//float LongRange = 0.f;				// ���Ÿ� �ν� �Ÿ�
	//int32 LongRangeDamage = 0;			// ���Ÿ� ���� �� ���� ������

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default|FSMComp")
	class UC_EnemyFSM* FSM;

public:
	bool bIsFlinched = false;
	bool bIsStaggered = false;
	bool bIsDead = false;

public:
	void CheckState();		// ����Ÿ��� ��������, ��Ʋ�Ÿ��� ��������, �׾����� üũ

	/***** Getter *****/
	float GetMeleeRange();
	//float GetLongRange();
	float GetSpeed();
};
