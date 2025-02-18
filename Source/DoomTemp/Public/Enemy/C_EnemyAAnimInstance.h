#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "C_EnemyFSM.h"
#include "C_EnemyAAnimInstance.generated.h"

UCLASS()
class DOOMTEMP_API UC_EnemyAAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	/***** Enemy Main state *****/
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FSM")
	EEnemyState MainAnimState = EEnemyState::MAX;


	/***** Enemy Sub state - movement *****/ 
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FSM")
	EEnemyMovement SubAnimMovement = EEnemyMovement::MAX;


	/***** Enemy Sub state - ���� ���� ��� ���� *****/
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FSM")
	bool bAttackPlay = false;


	/***** Attack Notify *****/
	UFUNCTION()
	void AnimNotify_EAttackEnd();
};
