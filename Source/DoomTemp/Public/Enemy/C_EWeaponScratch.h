#pragma once

#include "CoreMinimal.h"
#include "Enemy/C_EWeapon.h"
#include "C_EWeaponScratch.generated.h"

UCLASS()
class DOOMTEMP_API AC_EWeaponScratch : public AC_EWeapon
{
	GENERATED_BODY()
	
public:
	AC_EWeaponScratch();

	/***** Scratch Comp *****/
	// 할퀴기 충돌 처리 진행
	// 부착할 socket의 이름과 같게 명명
protected:
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Scratch Weapon")
	class USphereComponent* ScratchComp;

	// Overlap 이밴트 처리 함수
public:
	void OnEWeaponScratchOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
