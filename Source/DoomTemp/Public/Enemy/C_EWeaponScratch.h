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
	// ������ �浹 ó�� ����
	// ������ socket�� �̸��� ���� ���

protected:
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Scratch Weapon")
	class USphereComponent* Scratch_L;	
};
