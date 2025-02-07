#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_EWeapon.generated.h"

UCLASS(abstract)
class DOOMTEMP_API AC_EWeapon : public AActor
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName WeaponSocketName;

private:
    UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

protected:
	UPROPERTY(VisibleAnywhere)
	class USkeletalMesh* Mesh;

public:	
	AC_EWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	bool CanEquip();
	void Equip();
	void BeginEquip();
	void EndEquip();

	bool CanUnequip();
	void UnEquip();

private:
	class AC_Enemy* Owner;
};
