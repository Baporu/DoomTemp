#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_EWeapon.generated.h"


// abstract : Unreal Editor에서 추상 배이스 클래스로 취급하여 Map에 배치할 수 없음
UCLASS(abstract)
class DOOMTEMP_API AC_EWeapon : public AActor
{
	GENERATED_BODY()
	
protected:
	// Weapon을 부착할 소켓 이름
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
	bool CanEquip();	// 무기 장착 가능 상태인지 체크
	void Equip();		// 무기 장착 하고 있는 상태로 변경
	void BeginEquip();	// 무기 장착 시작
	void EndEquip();	// 무기 장착 끝난 상태로 변경

	bool CanUnequip();	// 무기 장착 해제 가능 상태인지 체크
	void UnEquip();		// 무기 장착 해제

private:
	bool bEquipping;	// 무기를 장착하고 있는가
	bool bFiring;		// 발사하고 있는가
	bool bReload;		// 재장전 중인가

protected:
	class AC_Enemy* Owner;
};
