#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "C_EnemyAAnimInstance.generated.h"

UCLASS()
class DOOMTEMP_API UC_EnemyAAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// Enemy ¼Óµµ
protected:
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "EnemyAnimInstance")
	float Speed = 0.f;

};
