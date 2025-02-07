//#pragma once
//
//#include "CoreMinimal.h"
//#include "Components/ActorComponent.h"
//#include "C_EWeaponComp.generated.h"
//
//UENUM(BlueprintType)
//enum class EEWeaponType : uint8
//{
//	SCRATCH,	// 할퀴기
//	FIREBALL,	// 파이어 볼
//	MAX			// 아무것도 안함
//};
//
////////////////// 여기 왜 에러가 나는 걸까
////DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EEWeaponType, InPrevType,// EEWeaponType, InNewType);
//
//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
//class DOOMTEMP_API UC_EWeaponComp : public UActorComponent
//{
//	GENERATED_BODY()
//
//private:
//    UPROPERTY(EditAnywhere, Category = "Settings")
//	TArray<TSubclassOf<class AC_EWeapon>> WeaponClass;
//
//public:
//	FORCEINLINE bool IsUnarmedMode() { return WeaponType == EEWeaponType::MAX; }
//	FORCEINLINE bool IsScratchMode() { return WeaponType == EEWeaponType::SCRATCH; }
//	FORCEINLINE bool IsFireballMode() { return WeaponType == EEWeaponType::FIREBALL; }
//
//public:	
//	UC_EWeaponComp();
//
//protected:
//	virtual void BeginPlay() override;
//
//public:	
//	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
//
//private:
//	AC_EWeapon* GetCurWeapon();
//	void SetUnarmedMode();
//	void SetMode(EEWeaponType InType);
//	void ChangeType(EEWeaponType InType);
//
//
////////////////// 여기 왜 에러가 나는 걸까
////public:
////	FWeaponTypeChanged OnWeaponTypeChanged;
//
//private:
//	EEWeaponType WeaponType = EEWeaponType::MAX;
//
//private:
//	class AC_Enemy* Owner;
//	TArray<class AC_EWeapon*> Weapons;
//};
