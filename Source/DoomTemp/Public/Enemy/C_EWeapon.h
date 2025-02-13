#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_EWeapon.generated.h"


// abstract : Unreal Editor���� �߻� ���̽� Ŭ������ ����Ͽ� Map�� ��ġ�� �� ����
UCLASS(abstract)
class DOOMTEMP_API AC_EWeapon : public AActor
{
	GENERATED_BODY()
	
protected:
	// Weapon�� ������ ���� �̸�
    UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName WeaponSocketName;

	//UPROPERTY(EditDefaultsOnly, Category = "Equip")
	//class UAnimMontage* EquipMontage;
	//
	//UPROPERTY(EditDefaultsOnly, Category = "Equip")
	//float EquipMontagePlayRate;


protected:
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USceneComponent* Root;

protected:
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Mesh;

public:	
	AC_EWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	bool CanEquip();	// ���� ���� ���� �������� üũ
	void Equip();		// ���� ���� �ϰ� �ִ� ���·� ����
	void BeginEquip();	// ���� ���� ����
	void EndEquip();	// ���� ���� ���� ���·� ����

	bool CanUnequip();	// ���� ���� ���� ���� �������� üũ
	void UnEquip();		// ���� ���� ����

private:
	bool bEquipping;	// ���⸦ �����ϰ� �ִ°�
	bool bFiring;		// �߻��ϰ� �ִ°�
	bool bReload;		// ������ ���ΰ�

protected:
	class AC_Enemy* Owner;
};
