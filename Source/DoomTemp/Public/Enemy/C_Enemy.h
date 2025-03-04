#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_Enemy.generated.h"

// abstract : Unreal Editor���� �߻� ���̽� Ŭ������ ����Ͽ� Map�� ��ġ�� �� ����
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
	
	int32 HPMax = 200;
	int32 HP = HPMax;

	float SpeedMax = 80.f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Info")
    float Speed = SpeedMax;

	int32 HPFlinched = HP * 0.5;		// ��Ʋ�Ÿ��� ����
	int32 HPStaggered = HP * 0.33;	// ����Ÿ��� ����

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Info")
	float MeleeRange = 100.f;			// �ٰŸ� �ν� �Ÿ�

	int32 MeleeDamageMax = 15;
	int32 MeleeDamage = MeleeDamageMax;				// �ٰŸ� ���� �� ���� ������
	int32 MeleeDamageFlinch = MeleeDamage * 0.6;	// Flinch ���¿��� ���� �ٰŸ� ���� ������


	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy|Info")
	//float LongRange = 0.f;				// ���Ÿ� �ν� �Ÿ�
	//int32 LongRangeDamage = 0;			// ���Ÿ� ���� �� ���� ������


	FVector HitPos;			// ������ ���� ���� ��ġ
	FRotator HitPointDir;	// �ǰ� �վ��� ���� ����


	/***** Materials *****/
	class UMaterial* MatFlinch;
	class UMaterial* MatStagger;


	/***** FSM *****/
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FSMComp")
	class UC_EnemyFSM* FSM;


	/***** Weapon *****/
	// Weapon component �ȿ� Weapon�� �����ϱ� Weapon Comp�� Weapon�� ���� ����
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
	// ����Ÿ��� ��������, ��Ʋ�Ÿ��� ��������, �׾����� üũ
	void CheckSubState();		

	void OnDamageProcess(int32 InDamage, enum class EAttackType InAttackType);
	void OnDamageProcess(int32 InDamage, enum class EAttackType InAttackType, FVector InHitPos, FVector InHitPointNormal);
	// Enemy ��� �� ó���� ��
	void OnDead();			

	// Enemy ���¿� ���� Speed ����
	void SetEnemySpeed();

	// Enemy ���¿� ���� Melee Attack Damage ����
	void ChangeMeleeDamage();	

	// Enemy�� ���� ����
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
	// Enemy Spawn VFX�� Spawn
	UFUNCTION(BlueprintImplementableEvent, Category = "VFX")
	void SpawnVFX();


	/***** Change Material *****/
	void SetFlinchMaterial();
	void SetStaggerMaterial();
};
