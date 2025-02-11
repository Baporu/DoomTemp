#include "Enemy/C_EnemyFSM.h"
#include "C_Helpers.h"
#include "C_PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/C_EnemyA.h"

#include "NavigationSystem.h"
#include <Runtime/AIModule/Classes/AITypes.h>
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"


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
	CheckNull(Self);
	MeleeRange = Self->GetMeleeRange();
	//LongRange = Self->GetLongRange();
	

	/***** AI Controller *****/
	MyAI = Cast<AAIController>(Self->GetController());
}


void UC_EnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMessage = UEnum::GetValueAsString(EnemyState);
	//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, DebugMessage);

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
	FString DebugMessage = UEnum::GetValueAsString(EnemyMovement);
	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Magenta, DebugMessage);


	/***** 이동 *****/
	FVector destination = Target->GetActorLocation();
	FVector dir = Target->GetActorLocation() - Self->GetActorLocation();
	Self->AddMovementInput(dir.GetSafeNormal());


	/*** 순찰 ***/
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
	
	// 타겟이 있을 때만 공격하도록 - 타겟이 죽었는데 공격하면 안되니까
	CheckNull(Target);

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
	// 1. Attack Type에 따른 애니메이션 및 이펙트 실행
	switch (AttackType)
	{
		case EAttackType::Fist:
			GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Damage FIST-----");
			break;
		case EAttackType::Gun:
			GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Damage GUN-----");
			break;
		case EAttackType::GloryKill:
			GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Damage GLORYKILL-----");
			break;
		case EAttackType::Chainsaw:
			GEngine->AddOnScreenDebugMessage(0, 1, FColor::Orange, L"-----Damage CHAINSAW-----");
			break;
	}

	// 2. Enemy 상태를 IDLE로 변경
	EnemyState = EEnemyState::IDLE;
}


void UC_EnemyFSM::DeadState()
{
	// 1. 죽음 처리시 할 일을 한다
	Self->OnDead();

	// 2. 시간이 흐르다가
	CurTime += GetWorld()->DeltaRealTimeSeconds;

	// 3. 경과 시간이 대기 시간을 초과하면
	if (CurTime > DestroyDelayTime)
	{
		// 4. Enemy를 제거시킨다
		Self->Destroy();
	}
}

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
