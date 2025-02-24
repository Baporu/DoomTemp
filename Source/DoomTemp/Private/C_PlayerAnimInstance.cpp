// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PlayerAnimInstance.h"
#include "C_PlayerCharacter.h"
#include "Components/SphereComponent.h"

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

void UC_PlayerAnimInstance::AnimNotify_OnPunchStart()
{
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Blue, TEXT("Punch Started"));
	UE_LOG(LogTemp, Warning, TEXT("Punch Started"));

	AC_PlayerCharacter* player = Cast<AC_PlayerCharacter>(TryGetPawnOwner());

	if (player && !(player->MeleeTarget)) {
		player->PunchComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
		player->PunchComp->SetVisibility(true);
	}
}

void UC_PlayerAnimInstance::AnimNotify_OnPunchEnd()
{
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Blue, TEXT("Punch Ended"));
	UE_LOG(LogTemp, Warning, TEXT("Punch Ended"));

	AC_PlayerCharacter* player = Cast<AC_PlayerCharacter>(TryGetPawnOwner());

	if (player) {
		bIsPunching = false;
		player->OnPunchEnd();
	}
}
