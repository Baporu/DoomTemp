#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_EWeaponComp.generated.h"

// BlueprintType을 쓰면 BP에서 보임
UENUM(BlueprintType)
enum class EEWeaponType : uint8
{
	SCRATCH,	// 할퀴기
	FIREBALL,	// 파이어 볼
	MAX			// 아무것도 안함
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EEWeaponType, InPrevType, EEWeaponType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOOMTEMP_API UC_EWeaponComp : public UActorComponent
{
	GENERATED_BODY()

	/***** BP에서 만든 AC_EWeapon을 요소로 갖는 TArray 배열 *****/
private:
	// Enemy가 소유한 weapon들
	// TSubclassOf<class AC_EWeapon> : AC_EWeapon의 파생 클래스를 TArray의 원소로 가짐
	// └ 즉, AC_EWeapon의 파생 클래스가 Enemy가 소유할 수 있는 Weapon이 됨
    UPROPERTY(EditAnywhere, Category = "Settings")
	TArray<TSubclassOf<class AC_EWeapon>> WeaponClasses;


	/***** 장착 중인 무기 상태 체크 *****/
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

	// 현재 장착한 무기 return
private:
	AC_EWeapon* GetCurWeapon();

	// 무기 장착 해제
public:
	void SetUnarmedMode();

private:
	// 무기 장착
	void SetMode(EEWeaponType InType);
	// 장착 중인 무기 상태 변경
	void ChangeType(EEWeaponType InType);

public:
	void BeginEquip();
	void EndEquip();

public:
	// Enemy 사망 시 Enemy가 들고 있던 모든 Weapon들 Destroy
	void WeaponDestroy();

public:
	FWeaponTypeChanged OnWeaponTypeChanged;

private:
	EEWeaponType WeaponType = EEWeaponType::MAX;	// 아무것도 선택 X

private:
	class AC_Enemy* Owner;	// Enemy
	TArray<class AC_EWeapon*> Weapons;	// 무기 배열 변수

public:
	TArray<class AC_EWeapon*> GetWeapons();
};
