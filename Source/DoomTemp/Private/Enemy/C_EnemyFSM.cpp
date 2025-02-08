#include "Enemy/C_EnemyFSM.h"
#include "C_Helpers.h"
#include "C_PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/C_EnemyA.h"

#include "NavigationSystem.h"
#include <Runtime/AIModule/Classes/AITypes.h>
#include <AIController.h>
#include "Navigation/PathFollowingComponent.h"


UC_EnemyFSM::UC_EnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UC_EnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	

	/***** Target & Self *****/
	auto actor = UGameplayStatics::GetActorOfClass( GetWorld(), AC_PlayerCharacter::StaticClass() );
	CheckNull(actor);
	Target = Cast<AC_PlayerCharacter>( actor );
	Self = Cast<AC_EnemyA>( GetOwner() );


	/***** Attack Range *****/
	if (!!Self)
	{
		MeleeRange = Self->GetMeleeRange();
		//LongRange = Self->GetLongRange();
	}
	

	/***** AI Controller *****/
	MyAI = Cast<AAIController>(Self->GetController());
}


void UC_EnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


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
		case EEnemyState::MAX:
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
	// 1. Spawn VFX�� ��ȯ�Ѵ�
	

	// 2. Spawn �ִϸ��̼��� ����Ѵ�

	// 3. Idle ���·� ��ȯ�Ѵ�
	EnemyState = EEnemyState::IDLE;
}

void UC_EnemyFSM::IdleState()
{
	CurTime += GetWorld()->DeltaTimeSeconds;

	if (CurTime > IdleDelayTime)
	{
		// �̵� ���·� ��ȯ
		EnemyState = EEnemyState::MOVE;
		CurTime = 0.f;

		// ���� ��ġ ���ϱ�
		GetRandomPositionInNavMesh( Self->GetActorLocation(), 500, RandPos);
	}
}


void UC_EnemyFSM::MoveState()
{
	/***** �⺻ ��������, ��Ʋ�Ÿ��� �������� üũ *****/
	#pragma region Check Movement
	////////////////////// �� �κ� �ǹ� //////////////////////
	//if (Self->bIsFlinched == true)
	//{
	//	// 1. �Ͻ������� �������� ��������
	//
	//	// 2. ���� ������ ��¦�δ�
	//}
	#pragma endregion
	//FString DebugMessage = UEnum::GetValueAsString(EnemyMove);
	//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Magenta, DebugMessage);

	/***** �̵� *****/
	FVector destination = Target->GetActorLocation();
	FVector dir = Target->GetActorLocation() - Self->GetActorLocation();
	Self->AddMovementInput(dir.GetSafeNormal());


	/*** Patrol ***/
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FPathFindingQuery query;
	FAIMoveRequest req;

	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);

	MyAI->BuildPathfindingQuery(req, query);

	FPathFindingResult rslt = ns->FindPathSync(query);


	// ������������ �� ã�� ���� ���� Ȯ��
	if (rslt.Result == ENavigationQueryResult::Success)
		MyAI->MoveToLocation(destination);
	else
	{
		auto result = MyAI->MoveToLocation(RandPos);
		if(result == EPathFollowingRequestResult::AlreadyAtGoal)
			GetRandomPositionInNavMesh(Self->GetActorLocation(), 500, RandPos);
	}


	/***** �ٰŸ� ���� ���·� ��ȯ *****/
	if ( dir.Size() <= MeleeRange )
	{
		MyAI->StopMovement();
		EnemyState = EEnemyState::ATTACK;
	}
}

void UC_EnemyFSM::AttackState()
{
	float distance = FVector::Distance( Target->GetActorLocation(), Self->GetActorLocation() );
	
	CurTime += GetWorld()->DeltaTimeSeconds;
	
	///// �߰� : Ÿ���� ���� ���� �����ϵ��� - Ÿ���� �׾��µ� �����ϸ� �ȵǴϱ�

	if (CurTime > AttackDelayTime)
	{
		// �ٰŸ� ������ �Ѵ�
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, L"Melee Attack!!!!!");

        //// ���Ÿ� ������ �Ѵ�
        //GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, L"Long Range Attack!!!!!");

		CurTime = 0.f;
	}
	
	// ���� Player�� �ٰŸ� �������� ����ٸ�
	if (distance > MeleeRange )
	{
		// ���¸� �̵����� ��ȯ�Ѵ�
		EnemyState = EEnemyState::MOVE;
		// ���� ��ġ ���ϱ�
		GetRandomPositionInNavMesh(Self->GetActorLocation(), 500, RandPos);
	}
}

void UC_EnemyFSM::DamageState()
{

}

void UC_EnemyFSM::DeadState()
{

}

EEnemyMove UC_EnemyFSM::GetEnemyMove()
{
	return EnemyMove;
}

void UC_EnemyFSM::SetEnemyDamaged(EEnemyDamaged InVal)
{
	EnemyDamaged = InVal;
}

void UC_EnemyFSM::SetEnemyMove(EEnemyMove InVal)
{
	EnemyMove = InVal;
}

