#include "Enemy/C_EnemyFSM.h"
#include "C_Helpers.h"
#include "C_PlayerCharacter.h"
#include "Enemy/C_EnemyA.h"
#include "Enemy/C_EnemyAAnimInstance.h"
#include "Kismet/GameplayStatics.h"

#include "NavigationSystem.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"
#include <Runtime/AIModule/Classes/AITypes.h>


UC_EnemyFSM::UC_EnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UC_EnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	EnemyState = EEnemyState::SPAWN;
	EnemyMovement = EEnemyMovement::MAX;

	/***** Target & Self *****/
	auto actor = UGameplayStatics::GetActorOfClass( GetWorld(), AC_PlayerCharacter::StaticClass() );
	CheckNull(actor);

	Target = Cast<AC_PlayerCharacter>( actor );
	Self = Cast<AC_EnemyA>( GetOwner() );

	/***** Attack Range *****/
	CheckNull(Self);
	MeleeRange = Self->GetMeleeRange();
	//LongRange = Self->GetLongRange();
	
	/***** AI Controller *****/
	MyAI = Cast<AAIController>(Self->GetController());

	/***** Animation *****/
	Anim = Cast<UC_EnemyAAnimInstance>(Self->GetMesh()->GetAnimInstance());
}


void UC_EnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTemp, Error, TEXT(">>> Enemy Main State: %s / Sub State : %s"), *UEnum::GetValueAsString(EnemyState), *UEnum::GetValueAsString(EnemyMovement));

	switch (EnemyState)
	{
		case EEnemyState::IDLE:
			IdleState();
			break;
		case EEnemyState::MOVE:
			MoveState();
			break;
		case EEnemyState::ATTACK:
			AttackState();
			break;
		case EEnemyState::DAMAGE:
			DamageState();
			break;
		case EEnemyState::DEAD:
			DeadState();
			break;
		case EEnemyState::SPAWN:
			SpawnState();
			break;
	}
}

bool UC_EnemyFSM::GetRandomPositionInNavMesh(FVector InCenterLocation, float InRadius, FVector& InDest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool rslt = ns->GetRandomReachablePointInRadius(InCenterLocation, InRadius, loc);
	InDest = loc.Location;

	return rslt;
}

void UC_EnemyFSM::SpawnState()
{
	// 1. ���� ��ġ ���ϱ�
	GetRandomPositionInNavMesh(Self->GetActorLocation(), 500, RandPos);

	// 2. �ִϸ��̼� ���
	Anim->MainAnimState = EnemyState;
	Anim->SubAnimMovement = EnemyMovement;

	// 3. Spawn VFX�� ��ȯ�Ѵ�

	// 4. Idle ���·� ��ȯ�Ѵ�
	EnemyState = EEnemyState::IDLE;
	EnemyMovement = EEnemyMovement::MAX;
	//CurTime = 0.f;	// IDLE���� ���ϰ� �ִ� CurTime �ʱ�ȭ
}

void UC_EnemyFSM::IdleState()
{
	CurTime += GetWorld()->DeltaTimeSeconds;

	if (CurTime > IdleDelayTime)
	{
		// 1. �̵� ���·� ��ȯ
		EnemyState = EEnemyState::MOVE;
		EnemyMovement = EEnemyMovement::WALK;

		// 2. ��� �ð� �ʱ�ȭ
		CurTime = 0.f;

		// 3. �ִϸ��̼� ���� ����ȭ
		Anim->MainAnimState = EnemyState;
		Anim->SubAnimMovement = EnemyMovement;
	}
}


void UC_EnemyFSM::MoveState()
{
	FString DebugMessage;

	/*** Sub state�� ���� �ൿ ���� ***/
	switch (EnemyMovement)
	{
	case EEnemyMovement::WALK:
		WalkMovement();
		break;
	case EEnemyMovement::FLINCH:
		FlinchMovement();
		break;
	case EEnemyMovement::STAGGER:
		StaggerMovement();
		break;
	}

	/*** Sub state�� ���� �ִϸ��̼� ����ȭ ***/
	Anim->MainAnimState = EnemyState;
	Anim->SubAnimMovement = EnemyMovement;
}


void UC_EnemyFSM::AttackState()
{
	/*** Ÿ���� �ִ��� üũ - Ÿ���� �׾��µ� �����ϸ� �ȵǴϱ� ***/
	CheckNull(Target);

	//if(Anim->bAttackPlay) return;

	/*** ���� �ð��� ������ ���� ***/
	CurTime += GetWorld()->DeltaTimeSeconds;

	// 1. ���� �ð��� �Ǹ� 
	if (CurTime > AttackDelayTime)
	{
		// 2. �ٰŸ� ������ �Ѵ�
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, L"Melee Attack!!!!!");

        //// ���Ÿ� ������ �Ѵ�
        //GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, L"Long Range Attack!!!!!");

		// 3. ��� �ð� �ʱ�ȭ
		CurTime = 0.f;

		// 4. animaiton ���� �÷��� ��ȯ
		Anim->bAttackPlay = true;
	}

	
	/*** �ٰŸ��� ����� MOVE ���·� ��ȯ ***/
	float distance = FVector::Distance( Target->GetActorLocation(), Self->GetActorLocation() );

	// ���� Player�� �ٰŸ� �������� ����ٸ�
	if (distance > MeleeRange)
	{
		// Debug
		//GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, L">>> Out /of /Distance");
		UE_LOG(LogTemp, Error, TEXT(">>> Player Out of Range"));
	
		// ���ο� ���� ��ġ ���ϱ�
		GetRandomPositionInNavMesh(Self->GetActorLocation(), 500, RandPos);

		// ���¸� �̵����� ��ȯ�Ѵ�
		EnemyState = EEnemyState::MOVE;
		EnemyMovement = EEnemyMovement::WALK;
	
		// �ִϸ��̼� ���� ����ȭ
		Anim->MainAnimState = EnemyState;
		Anim->SubAnimMovement = EnemyMovement;
	}
}

void UC_EnemyFSM::OnAttackEnd()
{
	Anim->bAttackPlay = false;
}


void UC_EnemyFSM::DamageState()
{
	// Attack Type�� ���� �ִϸ��̼� �� ����Ʈ ����
	switch (AttackType)
	{
		case EAttackType::Fist:
			Self->OnDamageFist();
			break;
		case EAttackType::Gun:
			Self->OnDamageGun();
			break;
		case EAttackType::GloryKill:
			Self->OnDamageGloryKill();
			break;
		case EAttackType::Chainsaw:
			Self->OnDamageChainsaw();
			break;
	}

	// �ð��� �帣�ٰ�
	CurTime += GetWorld()->DeltaTimeSeconds;

	// ��� �ð��� ��� �ð��� �ʰ��ϸ�
	if (CurTime > DamageDelayTime)
	{
		// Enemy ���¸� IDLE�� ����
		EnemyState = EEnemyState::IDLE;
		EnemyMovement = EEnemyMovement::MAX;

		// ��� �ð� �ʱ�ȭ
		CurTime = 0.f;

		// �ִϸ��̼� ���� ����ȭ
		Anim->MainAnimState = EnemyState;
		Anim->SubAnimMovement = EnemyMovement;
	}
}


void UC_EnemyFSM::DeadState()
{
	// 1. ���� ó���� �� ���� �Ѵ�
	Self->OnDead();

	// 2. �ð��� �帣�ٰ�
	CurTime += GetWorld()->DeltaRealTimeSeconds;

	// 3. ��� �ð��� ��� �ð��� �ʰ��ϸ�
	if (CurTime > DestroyDelayTime)
	{
		// 4. Enemy�� ���Ž�Ų��
		Self->Destroy();
	}
}


void UC_EnemyFSM::WalkMovement()
{
	/*** Debug ***/
	FString DebugMessage = UEnum::GetValueAsString(EnemyMovement);
	//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Magenta, DebugMessage);

	// player�� ���� �����δ�
	Move();
}

void UC_EnemyFSM::FlinchMovement()
{
	/*** Debug ***/
	FString DebugMessage = UEnum::GetValueAsString(EnemyMovement);
	//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Magenta, DebugMessage);

	// ���� �Ķ������� ��¦�δ�

}

void UC_EnemyFSM::StaggerMovement()
{
	/*** Debug ***/
	FString DebugMessage = UEnum::GetValueAsString(EnemyMovement);
	//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Magenta, DebugMessage);

	// ���� ���������� ��¦�δ�

}


/***** Getters *****/
EEnemyMovement UC_EnemyFSM::GetEnemyMovement()
{
	return EnemyMovement;
}

EEnemyState UC_EnemyFSM::GetEnemyState()
{
	return EnemyState;
}

AAIController* UC_EnemyFSM::GetMyAI()
{
	return MyAI;
}


/***** Setters *****/
void UC_EnemyFSM::SetEnemyMovement(EEnemyMovement InVal)
{
	EnemyMovement = InVal;
}

void UC_EnemyFSM::SetEnemyState(EEnemyState InVal)
{
	EnemyState = InVal;
}

void UC_EnemyFSM::SetAttackType(EAttackType InVal)
{
	AttackType = InVal;
}

void UC_EnemyFSM::SetAnimState(EEnemyState InVal)
{
	Anim->MainAnimState = InVal;
}

void UC_EnemyFSM::SetAnimSubStateMovement(EEnemyMovement InVal)
{
	Anim->SubAnimMovement = InVal;
}


void UC_EnemyFSM::Move()
{
	/*** �̵� : Walk ������ �� ���� ***/
	FVector destination = Target->GetActorLocation();
	FVector dir = Target->GetActorLocation() - Self->GetActorLocation();
	//Self->AddMovementInput(dir.GetSafeNormal());


	/*** ���� : Walk ������ �� ���� ***/
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FPathFindingQuery query;
	FAIMoveRequest req;

	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);

	MyAI->BuildPathfindingQuery(req, query);

	FPathFindingResult rslt = ns->FindPathSync(query);


	/*** ������������ �� ã�� ���� ���� Ȯ�� ***/
	if (rslt.Result == ENavigationQueryResult::Success)
		MyAI->MoveToLocation(destination);
	else
	{
		auto result = MyAI->MoveToLocation(RandPos);
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
			GetRandomPositionInNavMesh(Self->GetActorLocation(), 500, RandPos);
	}
	UE_LOG(LogTemp, Warning, TEXT(">>> Enemy Nav Move : %s"), *UEnum::GetValueAsString( rslt.Result ) );
	
	/*** �Ÿ��� ���� �ٰŸ� ���� ���·� ��ȯ ***/
	CheckDistance(dir.Size());
}


void UC_EnemyFSM::CheckDistance(float InDistance)
{
	/*** �Ÿ��� ���� �ٰŸ� ���� ���·� ��ȯ ***/
	if (InDistance < MeleeRange)
	{
		// 1. �������� �����
		MyAI->StopMovement();

		// 2. ���� ���·� ��ȯ
		EnemyState = EEnemyState::ATTACK;
		EnemyMovement = EEnemyMovement::MAX;

		// 3. �ִϸ��̼� ���� ����ȭ
		Anim->MainAnimState = EnemyState;
		Anim->SubAnimMovement = EnemyMovement;

		// 4. ���� �ִϸ��̼� ��� Ȱ��ȭ
		Anim->bAttackPlay = true;

		// 5. ���� ���� ��ȯ �� ��� �ð� ���� �ٷ� �÷��� �ǵ���
		CurTime = AttackDelayTime;
	}
}

// Animation Montage�� �����ϴ� �κ��� ���Ƽ� �Լ��� �������
void UC_EnemyFSM::PlayEnemyMontage(FString* InSectionName)
{
	if (InSectionName->Contains(TEXT("Damage")))
	{
		// Damage Animation Montage ����
		Self->PlayAnimMontage(Anim->EnemyMontage, 2.f, FName(*InSectionName));
	}
	else
	{
		// Spawn Animation Montage ����
		Self->PlayAnimMontage(Anim->EnemyMontageSpawn, 3.f, FName(*InSectionName));
	}
}
