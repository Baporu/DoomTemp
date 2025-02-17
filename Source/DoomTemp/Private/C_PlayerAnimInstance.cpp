// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PlayerAnimInstance.h"
#include "C_PlayerCharacter.h"

void UC_PlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AC_PlayerCharacter* player = Cast<AC_PlayerCharacter>(TryGetPawnOwner());

	if (!player)
		return;

	bIsFire = player->bIsFire;
}
