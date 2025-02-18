// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PlayerAnimInstance.h"
#include "C_PlayerCharacter.h"

void UC_PlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

// 	AC_PlayerCharacter* player = Cast<AC_PlayerCharacter>(TryGetPawnOwner());
// 
// 	if (!player)
// 		return;
}

void UC_PlayerAnimInstance::PlayShootAnim()
{
	if (!ShootMontage)
		return;

	Montage_Play(ShootMontage);
}

void UC_PlayerAnimInstance::AnimNotify_OnPunchEnd()
{
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Blue, TEXT("Punch Ended"));
	UE_LOG(LogTemp, Warning, TEXT("LOOOOOOOOOOOOOOG"));

	AC_PlayerCharacter* player = Cast<AC_PlayerCharacter>(TryGetPawnOwner());

	if (player) {
		bIsPunching = false;
		player->OnPunchEnd();
	}
}
