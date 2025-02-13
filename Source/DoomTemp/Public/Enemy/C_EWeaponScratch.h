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

	/***** Sphere Comp *****/
	// 할퀴기 충돌 처리 진행
	// 부착할 socket의 이름과 같게 명명

protected:
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Scratch Weapon")
	class USphereComponent* Scratch_L;	
};
