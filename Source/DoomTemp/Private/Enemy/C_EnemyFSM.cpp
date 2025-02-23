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
#include "Kismet/KismetSystemLibrary.h"


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

	/* 거리에 따른 Enemy Rotation 변경 */
	FVector destination = Target->GetActorLocation();
	FVector dir = Target->GetActorLocation() - Self->GetActorLocation();

	// 걸어가고 있는 상태에서 Player가 사정 거리 내에 들어오면
	if ( EnemyState != EEnemyState::DEAD && EnemyMovement != EEnemyMovement::FLINCH && EnemyMovement != EEnemyMovement::STAGGER && dir.Size() <= MeleeRange )
	{
		// 여기서부터 정연님이 알려주심
		FVector rot = destination - Self->GetActorLocation();

		FRotator TargetRotation = rot.Rotation();

		FRotator NewRotation = Self->GetActorRotation();
		NewRotation.Yaw = TargetRotation.Yaw; 
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
	// 랜덤 위치 정하기
	GetRandomPositionInNavMesh(Self->GetActorLocation(), 500, RandPos);

	// 애니메이션 재생
	Anim->MainAnimState = EnemyState;
	Anim->SubAnimMovement = EnemyMovement;

	// Spawn VFX를 소환한다

	// Idle 상태로 전환한다
	EnemyState = EEnemyState::IDLE;
	EnemyMovement = EEnemyMovement::MAX;
}

void UC_EnemyFSM::IdleState()
{
	CurTime += GetWorld()->DeltaTimeSeconds;

	if (CurTime >= IdleDelayTime)
	{
		// 이동 상태로 전환
		EnemyState = EEnemyState::MOVE;
		EnemyMovement = EEnemyMovement::WALK;

		// 경과 시간 초기화
		CurTime = 0.f;

		// Speed 변경
		Self->SetEnemySpeed();

		// 애니메이션 상태 동기화
		Anim->MainAnimState = EnemyState;
		Anim->SubAnimMovement = EnemyMovement;
	}
}


void UC_EnemyFSM::MoveState()
{
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


	/*** 일정 시간이 지나면 공격 ***/
	CurTime += GetWorld()->DeltaTimeSeconds;

	// 일정 시간이 되면 
	if (CurTime > AttackDelayTime)
	{
		// 근거리 공격을 한다
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, L"Melee Attack!!!!!");

        //// 원거리 공격을 한다
        //GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, L"Long Range Attack!!!!!");

		// 경과 시간 초기화
		CurTime = 0.f;

		// animaiton 공격 플래그 전환
		Anim->bAttackPlay = true;
	}
}

void UC_EnemyFSM::OnAttackEnd()
{
	Anim->bAttackPlay = false;

	/*** 근거리를 벗어나면 MOVE 상태로 전환 ***/
	float distance = FVector::Distance(Target->GetActorLocation(), Self->GetActorLocation());
	CanMove(distance);
}


void UC_EnemyFSM::DamageState()
{
	// 시간이 흐르다가
	CurTime += GetWorld()->DeltaTimeSeconds;
	//*** Debug
	UE_LOG(LogTemp, Warning, TEXT(">>>>> DamageState CurTime : %f"), CurTime);

	// 경과 시간이 대기 시간을 초과하면
	if (CurTime >= DamageDelayTime )
	{
		// Enemy Movement 상태에 따른 Main state 변경
		if (EnemyMovement == EEnemyMovement::FLINCH || EnemyMovement == EEnemyMovement::STAGGER)
		{
			EnemyState = EEnemyState::MOVE;
			// 경과 시간 초기화
			CurTime = IdleDelayTime;
			//*** Debug
			UE_LOG(LogTemp, Warning, TEXT(">>>>> DamageState CurTime : %f"), CurTime);
		}
		else
		{
			EnemyState = EEnemyState::IDLE; // 이동 중에 맞은 경우 or 정지 상태에서 맞은 경우
			EnemyMovement = EEnemyMovement::MAX;
			// 경과 시간 초기화
			CurTime = IdleDelayTime* 0.98;
			//*** Debug
			UE_LOG(LogTemp, Warning, TEXT(">>>>> DamageState CurTime : %f"), CurTime);
		}
		

		Self->CheckSubState();

	
		// 애니메이션 상태 동기화
		Anim->MainAnimState = EnemyState;
		Anim->SubAnimMovement = EnemyMovement;
	}
}


void UC_EnemyFSM::DeadState()
{
	// 애니메이션이 끝나면 사라지도록 한다
	if( !bDeadDone ) return;
	
	// 시간이 흐르다가
	CurTime += GetWorld()->DeltaRealTimeSeconds;
	
	// 경과 시간이 대기 시간을 초과하면
	if (CurTime > DestroyDelayTime)
	{
		// Enemy를 제거시킨다
		Self->Destroy();
	}
}

void UC_EnemyFSM::onDeadEnd() { bDeadDone = true; }


void UC_EnemyFSM::WalkMovement()
{
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
	
	/*** 거리에 따른 근거리 공격 상태로 전환 ***/
	CanMeleeAttack(dir.Size());
}


void UC_EnemyFSM::CanMeleeAttack(float InDistance)
{
	/*** 거리에 따른 근거리 공격 상태로 전환 ***/
	if (InDistance < MeleeRange)
	{
		// 움직임을 멈춘다
		MyAI->StopMovement();

		// 공격 상태로 전환
		EnemyState = EEnemyState::ATTACK;
		EnemyMovement = EEnemyMovement::MAX;

		// 애니메이션 상태 동기화
		Anim->MainAnimState = EnemyState;
		Anim->SubAnimMovement = EnemyMovement;

		// 공격 애니메이션 재생 활성화
		Anim->bAttackPlay = true;

		// 공격 상태 전환 시 대기 시간 없이 바로 플레이 되도록
		CurTime = AttackDelayTime;
	}
}

// Animation Montage를 실행하는 부분이 많아서 함수로 제작
void UC_EnemyFSM::PlayEnemyMontage(FString* InSectionName)
{
	if (InSectionName->Contains(TEXT("Damage")))
	{
		// Damage Animation Montage 실행
		Self->PlayAnimMontage(Anim->EnemyMontage, 2.f, FName(*InSectionName));
	}
	else
	{
		// Dead Animation Montage 실행
		Self->PlayAnimMontage(Anim->EnemyMontage, 3.f, FName(*InSectionName));
	}
}

void UC_EnemyFSM::CanMove(float InDistance)
{
	// 만약 Player가 근거리 범위에서 벗어났다면
	if (InDistance > MeleeRange)
	{
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

// Enemy Main state 및 Movement state 변경
void UC_EnemyFSM::ChangeEnemyStates(EEnemyState InMain, EEnemyMovement InMovement)
{
	EnemyState = InMain;
	EnemyMovement = InMovement;
}

// Enemy Movement 상태에 따른 Damage Animation Montage 재생
void UC_EnemyFSM::PlayDamageAM(FString* InSectionName)
{
	if (EnemyMovement == EEnemyMovement::WALK)
		PlayEnemyMontage(InSectionName);
}
