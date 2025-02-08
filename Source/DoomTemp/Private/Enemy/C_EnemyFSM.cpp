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
	// 1. Spawn VFX를 소환한다
	

	// 2. Spawn 애니메이션을 재생한다

	// 3. Idle 상태로 전환한다
	EnemyState = EEnemyState::IDLE;
}

void UC_EnemyFSM::IdleState()
{
	CurTime += GetWorld()->DeltaTimeSeconds;

	if (CurTime > IdleDelayTime)
	{
		// 이동 상태로 전환
		EnemyState = EEnemyState::MOVE;
		CurTime = 0.f;

		// 랜덤 위치 정하기
		GetRandomPositionInNavMesh( Self->GetActorLocation(), 500, RandPos);
	}
}


void UC_EnemyFSM::MoveState()
{
	/***** 기본 상태인지, 비틀거리는 상태인지 체크 *****/
	#pragma region Check Movement
	////////////////////// 이 부분 의문 //////////////////////
	//if (Self->bIsFlinched == true)
	//{
	//	// 1. 일시적으로 움직임이 더뎌진다
	//
	//	// 2. 몸이 빨갛게 반짝인다
	//}
	#pragma endregion
	//FString DebugMessage = UEnum::GetValueAsString(EnemyMove);
	//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Magenta, DebugMessage);

	/***** 이동 *****/
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


	// 목적지까지의 길 찾기 성공 여부 확인
	if (rslt.Result == ENavigationQueryResult::Success)
		MyAI->MoveToLocation(destination);
	else
	{
		auto result = MyAI->MoveToLocation(RandPos);
		if(result == EPathFollowingRequestResult::AlreadyAtGoal)
			GetRandomPositionInNavMesh(Self->GetActorLocation(), 500, RandPos);
	}


	/***** 근거리 공격 상태로 전환 *****/
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
	
	///// 추가 : 타겟이 있을 때만 공격하도록 - 타겟이 죽었는데 공격하면 안되니까

	if (CurTime > AttackDelayTime)
	{
		// 근거리 공격을 한다
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, L"Melee Attack!!!!!");

        //// 원거리 공격을 한다
        //GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, L"Long Range Attack!!!!!");

		CurTime = 0.f;
	}
	
	// 만약 Player가 근거리 범위에서 벗어났다면
	if (distance > MeleeRange )
	{
		// 상태를 이동으로 전환한다
		EnemyState = EEnemyState::MOVE;
		// 랜덤 위치 정하기
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

