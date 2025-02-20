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
	// 1. 랜덤 위치 정하기
	GetRandomPositionInNavMesh(Self->GetActorLocation(), 500, RandPos);

	// 2. 애니메이션 재생
	Anim->MainAnimState = EnemyState;
	Anim->SubAnimMovement = EnemyMovement;

	// 3. Spawn VFX를 소환한다

	// 4. Idle 상태로 전환한다
	EnemyState = EEnemyState::IDLE;
	EnemyMovement = EEnemyMovement::MAX;
	//CurTime = 0.f;	// IDLE에서 더하고 있던 CurTime 초기화
}

void UC_EnemyFSM::IdleState()
{
	CurTime += GetWorld()->DeltaTimeSeconds;

	if (CurTime > IdleDelayTime)
	{
		// 1. 이동 상태로 전환
		EnemyState = EEnemyState::MOVE;
		EnemyMovement = EEnemyMovement::WALK;

		// 2. 경과 시간 초기화
		CurTime = 0.f;

		// 3. 애니메이션 상태 동기화
		Anim->MainAnimState = EnemyState;
		Anim->SubAnimMovement = EnemyMovement;
	}
}


void UC_EnemyFSM::MoveState()
{
	FString DebugMessage;

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

	/*** Sub state에 따른 애니메이션 동기화 ***/
	Anim->MainAnimState = EnemyState;
	Anim->SubAnimMovement = EnemyMovement;
}


void UC_EnemyFSM::AttackState()
{
	/*** 타겟이 있는지 체크 - 타겟이 죽었는데 공격하면 안되니까 ***/
	CheckNull(Target);

	//if(Anim->bAttackPlay) return;

	/*** 일정 시간이 지나면 공격 ***/
	CurTime += GetWorld()->DeltaTimeSeconds;

	// 1. 일정 시간이 되면 
	if (CurTime > AttackDelayTime)
	{
		// 2. 근거리 공격을 한다
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, L"Melee Attack!!!!!");

        //// 원거리 공격을 한다
        //GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, L"Long Range Attack!!!!!");

		// 3. 경과 시간 초기화
		CurTime = 0.f;

		// 4. animaiton 공격 플래그 전환
		Anim->bAttackPlay = true;
	}

	
	/*** 근거리를 벗어나면 MOVE 상태로 전환 ***/
	float distance = FVector::Distance( Target->GetActorLocation(), Self->GetActorLocation() );

	// 만약 Player가 근거리 범위에서 벗어났다면
	if (distance > MeleeRange)
	{
		// Debug
		//GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, L">>> Out /of /Distance");
		UE_LOG(LogTemp, Error, TEXT(">>> Player Out of Range"));
	
		// 새로운 랜덤 위치 정하기
		GetRandomPositionInNavMesh(Self->GetActorLocation(), 500, RandPos);

		// 상태를 이동으로 전환한다
		EnemyState = EEnemyState::MOVE;
		EnemyMovement = EEnemyMovement::WALK;
	
		// 애니메이션 상태 동기화
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
	// Attack Type에 따른 애니메이션 및 이펙트 실행
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

	// 시간이 흐르다가
	CurTime += GetWorld()->DeltaTimeSeconds;

	// 경과 시간이 대기 시간을 초과하면
	if (CurTime > DamageDelayTime)
	{
		// Enemy 상태를 IDLE로 변경
		EnemyState = EEnemyState::IDLE;
		EnemyMovement = EEnemyMovement::MAX;

		// 경과 시간 초기화
		CurTime = 0.f;

		// 애니메이션 상태 동기화
		Anim->MainAnimState = EnemyState;
		Anim->SubAnimMovement = EnemyMovement;
	}
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
	/*** Debug ***/
	FString DebugMessage = UEnum::GetValueAsString(EnemyMovement);
	//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Magenta, DebugMessage);

	// player를 향해 움직인다
	Move();
}

void UC_EnemyFSM::FlinchMovement()
{
	/*** Debug ***/
	FString DebugMessage = UEnum::GetValueAsString(EnemyMovement);
	//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Magenta, DebugMessage);

	// 몸이 파란색으로 반짝인다

}

void UC_EnemyFSM::StaggerMovement()
{
	/*** Debug ***/
	FString DebugMessage = UEnum::GetValueAsString(EnemyMovement);
	//GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Magenta, DebugMessage);

	// 몸이 빨간색으로 반짝인다

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
	/*** 이동 : Walk 상태일 때 가능 ***/
	FVector destination = Target->GetActorLocation();
	FVector dir = Target->GetActorLocation() - Self->GetActorLocation();
	//Self->AddMovementInput(dir.GetSafeNormal());


	/*** 순찰 : Walk 상태일 때 가능 ***/
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
	UE_LOG(LogTemp, Warning, TEXT(">>> Enemy Nav Move : %s"), *UEnum::GetValueAsString( rslt.Result ) );
	
	/*** 거리에 따른 근거리 공격 상태로 전환 ***/
	CheckDistance(dir.Size());
}


void UC_EnemyFSM::CheckDistance(float InDistance)
{
	/*** 거리에 따른 근거리 공격 상태로 전환 ***/
	if (InDistance < MeleeRange)
	{
		// 1. 움직임을 멈춘다
		MyAI->StopMovement();

		// 2. 공격 상태로 전환
		EnemyState = EEnemyState::ATTACK;
		EnemyMovement = EEnemyMovement::MAX;

		// 3. 애니메이션 상태 동기화
		Anim->MainAnimState = EnemyState;
		Anim->SubAnimMovement = EnemyMovement;

		// 4. 공격 애니메이션 재생 활성화
		Anim->bAttackPlay = true;

		// 5. 공격 상태 전환 시 대기 시간 없이 바로 플레이 되도록
		CurTime = AttackDelayTime;
	}
}

// Animation Montage를 실행하는 부분이 많아서 함수로 만들었음
void UC_EnemyFSM::PlayEnemyMontage(FString* InSectionName)
{
	if (InSectionName->Contains(TEXT("Damage")))
	{
		// Damage Animation Montage 실행
		Self->PlayAnimMontage(Anim->EnemyMontage, 2.f, FName(*InSectionName));
	}
	else
	{
		// Spawn Animation Montage 실행
		Self->PlayAnimMontage(Anim->EnemyMontageSpawn, 3.f, FName(*InSectionName));
	}
}
