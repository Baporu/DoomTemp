#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_EWeaponComp.generated.h"

// BlueprintType�� ���� BP���� ����
UENUM(BlueprintType)
enum class EEWeaponType : uint8
{
	SCRATCH,	// ������
	FIREBALL,	// ���̾� ��
	MAX			// �ƹ��͵� ����
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EEWeaponType, InPrevType, EEWeaponType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOOMTEMP_API UC_EWeaponComp : public UActorComponent
{
	GENERATED_BODY()

	/***** BP���� ���� AC_EWeapon�� ��ҷ� ���� TArray �迭 *****/
private:
	// Enemy�� ������ weapon���� �迭
	// Spawn �� class type�� TSubclassOf�� ���
	// TArray : �װͿ� ���� �迭
    UPROPERTY(EditAnywhere, Category = "Settings")
	TArray<TSubclassOf<class AC_EWeapon>> WeaponClasses;

	/***** ���� ���� ���� ���� üũ *****/
public:
	FORCEINLINE bool IsUnarmedMode() { return WeaponType == EEWeaponType::MAX; }
	FORCEINLINE bool IsScratchMode() { return WeaponType == EEWeaponType::SCRATCH; }
	//FORCEINLINE bool IsFireballMode() { return WeaponType == EEWeaponType::FIREBALL; }

public:	
	UC_EWeaponComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ���� ������ ���� return
private:
	AC_EWeapon* GetCurWeapon();

	// ���� ���� ����
public:
	void SetUnarmedMode();

private:
	// ���� ����
	void SetMode(EEWeaponType InType);
	// ���� ���� ���� ���� ����
	void ChangeType(EEWeaponType InType);

public:
	void BeginEquip();
	void EndEquip();

public:
	FWeaponTypeChanged OnWeaponTypeChanged;

private:
	EEWeaponType WeaponType = EEWeaponType::MAX;	// �ƹ��͵� ���� X

private:
	class AC_Enemy* Owner;	// Enemy
	TArray<class AC_EWeapon*> Weapons;	// ���� �迭 ����
};
