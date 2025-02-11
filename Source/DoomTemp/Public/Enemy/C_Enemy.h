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
    // Default 값 : EnemyA를 기준으로
	
	int32 MaxHP = 200;
	int32 HP = MaxHP;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Info")
	float Speed = 180.f;

	int32 HPFlinched = HP * 0.5;		// 비틀거리는 상태
	int32 HPStaggered = HP * 0.33;	// 주춤거리는 상태

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Info")
	float MeleeRange = 100.f;			// 근거리 인식 거리
	int32 MeleeDamage = 15;				// 근거리 공격 시 가할 데미지

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy|Info")
	//float LongRange = 0.f;				// 원거리 인식 거리
	//int32 LongRangeDamage = 0;			// 원거리 공격 시 가할 데미지


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
	void CheckState();		// 주춤거리는 상태인지, 비틀거리는 상태인지, 죽었는지 체크
	void OnDamaged(int32 InDamage, enum class EAttackType );
	void OnDead();			// Enemy 사망 시 처리할 일
};
