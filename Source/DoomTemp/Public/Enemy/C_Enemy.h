#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_Enemy.generated.h"

// abstract : Unreal Editor에서 추상 배이스 클래스로 취급하여 Map에 배치할 수 없음
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


	FVector HitPos;			// 데미지 입은 곳의 위치
	FRotator HitPointDir;	// 피가 뿜어져 나갈 방향


	/***** Materials *****/
	class UMaterial* MatFlinch;
	class UMaterial* MatStagger;


	/***** FSM *****/
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FSMComp")
	class UC_EnemyFSM* FSM;


	/***** Weapon *****/
	// Weapon component 안에 Weapon이 있으니까 Weapon Comp로 Weapon에 접근 가능
    UPROPERTY(VisibleAnywhere)
	class UC_EWeaponComp* WeaponComps;
	#pragma endregion


	/***** Animation Montage *****/
	FString SectionName;


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
	UC_EWeaponComp* GetWeaponComps();
	
	/***** Setters *****/
protected:
	void SetHP(int32 InVal);
	void SetMeleeDamage(int32 InVal);
	void SetSpeed(float InVal);


	/***** Functions *****/
public:
	// 주춤거리는 상태인지, 비틀거리는 상태인지, 죽었는지 체크
	void CheckSubState();		

	void OnDamageProcess(int32 InDamage, enum class EAttackType InAttackType);
	void OnDamageProcess(int32 InDamage, enum class EAttackType InAttackType, FVector InHitPos, FVector InHitPointNormal);
	// Enemy 사망 시 처리할 일
	void OnDead();			

	// Enemy 상태에 따른 Speed 변경
	void SetEnemySpeed();

	// Enemy 상태에 따른 Melee Attack Damage 변경
	void ChangeMeleeDamage();	

	// Enemy의 방향 지정
	//void ChangeRotation();


	/***** Damage Events *****/
public:
	void OnDamageFist();
	void OnDamageGun();
	void OnDamageGloryKill();
	void OnDamageChainsaw();


	// Written By SHS
public:
	UPROPERTY(EditDefaultsOnly, Category = Drops)
	TArray<TSubclassOf<class ADropBase>> DropTypes;
	UPROPERTY(EditDefaultsOnly, Category = Drops)
	TArray<int32> DropCounts;

	void SpawnDrops();


	/***** Niagara *****/
	// Fist
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* NiagaraCompFist;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Niagara")
	class UNiagaraSystem* NiagaraSysFist;


	// Gun
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* NiagaraCompGun;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Niagara")
	class UNiagaraSystem* NiagaraSysGun;


	// Glory Kill
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* NiagaraCompGloryKill;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Niagara")
	class UNiagaraSystem* NiagaraSysGloryKill;


	// Chainsaw
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* NiagaraCompChainsaw;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Niagara")
	class UNiagaraSystem* NiagaraSysChainsaw;


	// Spawn Niagara
	UFUNCTION()
	void SpawnNiagara(class UNiagaraComponent* InComp, class UNiagaraSystem* InSys, FVector InScale = FVector(1.f));


	// Deactivate Niagara
	UFUNCTION()
	void DeActivateNiagara(class UNiagaraComponent* InComp);


	/***** Spawn *****/
	// Enemy Spawn VFX를 Spawn
	UFUNCTION(BlueprintImplementableEvent, Category = "VFX")
	void SpawnVFX();


	/***** Change Material *****/
	void SetFlinchMaterial();
	void SetStaggerMaterial();
};
