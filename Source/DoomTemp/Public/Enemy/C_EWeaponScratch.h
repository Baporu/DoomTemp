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
	// ������ �浹 ó�� ����
	// ������ socket�� �̸��� ���� ���
protected:
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Scratch Weapon")
	class USphereComponent* ScratchComp;

	// Overlap �̹�Ʈ ó�� �Լ�
public:
	void OnEWeaponScratchOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
