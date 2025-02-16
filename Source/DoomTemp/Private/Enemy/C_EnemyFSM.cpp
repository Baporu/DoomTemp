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
	// 1. 랜덤 위치 정하기
	GetRandomPositionInNavMesh(Self->GetActorLocation(), 500, RandPos);

	// 2. Spawn VFX를 소환한다
	

	// 3. Spawn 애니메이션을 재생한다


	// 3. Idle 상태로 전환한다
	EnemyState = EEnemyState::IDLE;
	EnemyMovement = EEnemyMovement::MAX;
}

void UC_EnemyFSM::IdleState()
{
	CurTime += GetWorld()->DeltaTimeSeconds;

	if (CurTime > IdleDelayTime)
	{
		// 이동 상태로 전환
		EnemyState = EEnemyState::MOVE;
		EnemyMovement = EEnemyMovement::WALK;
		CurTime = 0.f;
	}
}


void UC_EnemyFSM::MoveState()
{
	/*** Debug ***/
    FString DebugMessage = UEnum::GetValueAsString(EnemyMovement);
    GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Magenta, DebugMessage);


	/*** Sub state인 Movement에 따른 Speed 설정 ***/
	Self->SetEnemySpeed();


	/*** Sub state에 따른 행동 실행 ***/
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

	///*** 이동 : Walk, Flinch 상태일 때 가능 ***/
	//FVector destination = Target->GetActorLocation();
	//FVector dir = Target->GetActorLocation() - Self->GetActorLocation();
	//Self->AddMovementInput(dir.GetSafeNormal());
	
	
	/*** 순찰 : Walk, Flinch 상태일 때 가능 ***/
	//auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	//FPathFindingQuery query;
	//FAIMoveRequest req;
	//
	//req.SetAcceptanceRadius(3);
	//req.SetGoalLocation(destination);
	//
	//MyAI->BuildPathfindingQuery(req, query);
	//
	//FPathFindingResult rslt = ns->FindPathSync(query);
	//
	//
	///*** 목적지까지의 길 찾기 성공 여부 확인 ***/
	//if (rslt.Result == ENavigationQueryResult::Success)
	//	MyAI->MoveToLocation(destination);
	//else
	//{
	//	auto result = MyAI->MoveToLocation(RandPos);
	//	if(result == EPathFollowingRequestResult::AlreadyAtGoal)
	//		GetRandomPositionInNavMesh(Self->GetActorLocation(), 500, RandPos);
	//}


	///*** 근거리 공격 상태로 전환 ***/
	//if ( dir.Size() <= MeleeRange )
	//{
	//	MyAI->StopMovement();
	//	EnemyState = EEnemyState::ATTACK;
	//}
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
		EnemyMovement = EEnemyMovement::WALK;
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

	// 2. Enemy 상태를 IDLE로 변경
	EnemyState = EEnemyState::IDLE;
	EnemyMovement = EEnemyMovement::WALK;
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


void UC_EnemyFSM::WalkMovement()
{
	// player를 향해 움직인다
	Move();
}

void UC_EnemyFSM::FlinchMovement()
{
	// 몸이 파란색으로 반짝인다

	// player를 향해 움직인다
	Move();
}

void UC_EnemyFSM::StaggerMovement()
{
	// 몸이 빨간색으로 움직인다

}


/***** Getters *****/
EEnemyMovement UC_EnemyFSM::GetEnemyMovement()
{
	return EnemyMovement;
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

void UC_EnemyFSM::Move()
{
	/*** 이동 : Walk, Flinch 상태일 때 가능 ***/
	FVector destination = Target->GetActorLocation();
	FVector dir = Target->GetActorLocation() - Self->GetActorLocation();
	Self->AddMovementInput(dir.GetSafeNormal());

	/*** 순찰 : Walk, Flinch 상태일 때 가능 ***/
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FPathFindingQuery query;
	FAIMoveRequest req;

	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);

	MyAI->BuildPathfindingQuery(req, query);

	FPathFindingResult rslt = ns->FindPathSync(query);


	/*** 목적지까지의 길 찾기 성공 여부 확인 ***/
	if (rslt.Result == ENavigationQueryResult::Success)
		MyAI->MoveToLocation(destination);
	else
	{
		auto result = MyAI->MoveToLocation(RandPos);
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
			GetRandomPositionInNavMesh(Self->GetActorLocation(), 500, RandPos);
	}


	/*** 거리에 따른 근거리 공격 상태로 전환 ***/
	CheckDistance(dir.Size());
}


void UC_EnemyFSM::CheckDistance(float InDistance)
{
	/*** 거리에 따른 근거리 공격 상태로 전환 ***/
	if (InDistance <= MeleeRange)
	{
		MyAI->StopMovement();
		EnemyState = EEnemyState::ATTACK;
		EnemyMovement = EEnemyMovement::MAX;
	}
}
