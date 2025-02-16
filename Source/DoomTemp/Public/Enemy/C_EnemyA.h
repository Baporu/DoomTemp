#pragma once

#include "CoreMinimal.h"
#include "Enemy/C_Enemy.h"
#include "C_EnemyA.generated.h"


UCLASS()
class DOOMTEMP_API AC_EnemyA : public AC_Enemy
{
	GENERATED_BODY()
	
public:
	AC_EnemyA();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

//protected:
//	void SetMeleeDamage();
//	void SetSpeed();
};
