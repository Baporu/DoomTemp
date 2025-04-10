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
	EEnemyState MainAnimState = EEnemyState::IDLE;


	/***** Enemy Sub state - movement *****/ 
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FSM")
	EEnemyMovement SubAnimMovement = EEnemyMovement::MAX;


	/***** Enemy Sub state - 공격 상태 재생 여부 *****/
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FSM")
	bool bAttackPlay = false;


	/***** Animation Montage *****/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FSM")
	class UAnimMontage* EnemyMontage;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FSM")
	class UAnimMontage* EnemyMontageSpawn;


	/***** Attack Notify *****/
	UFUNCTION()
	void AnimNotify_EAttackStart();

	UFUNCTION()
	void AnimNotify_EAttackEnd();

	UFUNCTION()
	void AnimNotify_DeadEnd();


	/***** Spawn Notify *****/
	UFUNCTION()
	void AnimNotify_SpawnEnd();


	/***** Damage Notify *****/
	UFUNCTION()
	void AnimNotify_ChangeDamageRateStart();

	UFUNCTION()
	void AnimNotify_ChangeDamageRateEnd();


	/***** Change Player Collision Channel's setting *****/
private:
	class AC_EnemyA* Enemy;
	void ChangePlayerChannelCollision(ECollisionResponse InResponse);
};
