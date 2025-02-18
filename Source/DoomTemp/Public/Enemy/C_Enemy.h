#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_Enemy.generated.h"

// abstract : Unreal Editor에서 추상 배이스 클래스로 취급하여 Map에 배치할 수 없음
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
    // Default 값 : EnemyA를 기준으로
	
	int32 HPMax = 200;
	int32 HP = HPMax;

	float SpeedMax = 80.f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Info")
    float Speed = SpeedMax;

	int32 HPFlinched = HP * 0.5;		// 비틀거리는 상태
	int32 HPStaggered = HP * 0.33;	// 주춤거리는 상태

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Info")
	float MeleeRange = 100.f;			// 근거리 인식 거리

	int32 MeleeDamageMax = 15;
	int32 MeleeDamage = MeleeDamageMax;				// 근거리 공격 시 가할 데미지
	int32 MeleeDamageFlinch = MeleeDamage * 0.6;	// Flinch 상태에서 가할 근거리 공격 데미지


	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy|Info")
	//float LongRange = 0.f;				// 원거리 인식 거리
	//int32 LongRangeDamage = 0;			// 원거리 공격 시 가할 데미지


	/***** FSM *****/
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FSMComp")
	class UC_EnemyFSM* FSM;


	/***** Weapon *****/
	// Weapon component 안에 Weapon이 있으니까 Weapon Comp로 Weapon에 접근 가능
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
	
	UC_EnemyFSM* GetEnemyFSM();
	
	/***** Setters *****/
protected:
	void SetHP(int32 InVal);
	void SetMeleeDamage(int32 InVal);
	void SetSpeed(float InVal);


	/***** Functions *****/
public:
	void CheckSubState();		// 주춤거리는 상태인지, 비틀거리는 상태인지, 죽었는지 체크
	void OnDamageProcess(int32 InDamage, enum class EAttackType InAttackType);
	void OnDead();			// Enemy 사망 시 처리할 일

	void SetEnemySpeed();			// Enemy 상태에 따른 Speed 변경
	void ChangeMeleeDamage();	// Enemy 상태에 따른 Melee Attack Damage 변경


	/***** Damage Events *****/
public:
	void OnDamageFist();
	void OnDamageGun();
	void OnDamageGloryKill();
	void OnDamageChainsaw();
};
