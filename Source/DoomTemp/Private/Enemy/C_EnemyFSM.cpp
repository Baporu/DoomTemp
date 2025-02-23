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

	/* �Ÿ��� ���� Enemy Rotation ���� */
	FVector destination = Target->GetActorLocation();
	FVector dir = Target->GetActorLocation() - Self->GetActorLocation();

	// �ɾ�� �ִ� ���¿��� Player�� ���� �Ÿ� ���� ������
	if ( EnemyState != EEnemyState::DEAD && EnemyMovement != EEnemyMovement::FLINCH && EnemyMovement != EEnemyMovement::STAGGER && dir.Size() <= MeleeRange )
	{
		// ���⼭���� �������� �˷��ֽ�
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
	// ���� ��ġ ���ϱ�
	GetRandomPositionInNavMesh(Self->GetActorLocation(), 500, RandPos);

	// �ִϸ��̼� ���
	Anim->MainAnimState = EnemyState;
	Anim->SubAnimMovement = EnemyMovement;

	// Spawn VFX�� ��ȯ�Ѵ�

	// Idle ���·� ��ȯ�Ѵ�
	EnemyState = EEnemyState::IDLE;
	EnemyMovement = EEnemyMovement::MAX;
}

void UC_EnemyFSM::IdleState()
{
	CurTime += GetWorld()->DeltaTimeSeconds;

	if (CurTime >= IdleDelayTime)
	{
		// �̵� ���·� ��ȯ
		EnemyState = EEnemyState::MOVE;
		EnemyMovement = EEnemyMovement::WALK;

		// ��� �ð� �ʱ�ȭ
		CurTime = 0.f;

		// Speed ����
		Self->SetEnemySpeed();

		// �ִϸ��̼� ���� ����ȭ
		Anim->MainAnimState = EnemyState;
		Anim->SubAnimMovement = EnemyMovement;
	}
}


void UC_EnemyFSM::MoveState()
{
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


	/*** ���� �ð��� ������ ���� ***/
	CurTime += GetWorld()->DeltaTimeSeconds;

	// ���� �ð��� �Ǹ� 
	if (CurTime > AttackDelayTime)
	{
		// �ٰŸ� ������ �Ѵ�
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, L"Melee Attack!!!!!");

        //// ���Ÿ� ������ �Ѵ�
        //GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, L"Long Range Attack!!!!!");

		// ��� �ð� �ʱ�ȭ
		CurTime = 0.f;

		// animaiton ���� �÷��� ��ȯ
		Anim->bAttackPlay = true;
	}
}

void UC_EnemyFSM::OnAttackEnd()
{
	Anim->bAttackPlay = false;

	/*** �ٰŸ��� ����� MOVE ���·� ��ȯ ***/
	float distance = FVector::Distance(Target->GetActorLocation(), Self->GetActorLocation());
	CanMove(distance);
}


void UC_EnemyFSM::DamageState()
{
	// �ð��� �帣�ٰ�
	CurTime += GetWorld()->DeltaTimeSeconds;
	//*** Debug
	UE_LOG(LogTemp, Warning, TEXT(">>>>> DamageState CurTime : %f"), CurTime);

	// ��� �ð��� ��� �ð��� �ʰ��ϸ�
	if (CurTime >= DamageDelayTime )
	{
		// Enemy Movement ���¿� ���� Main state ����
		if (EnemyMovement == EEnemyMovement::FLINCH || EnemyMovement == EEnemyMovement::STAGGER)
		{
			EnemyState = EEnemyState::MOVE;
			// ��� �ð� �ʱ�ȭ
			CurTime = IdleDelayTime;
			//*** Debug
			UE_LOG(LogTemp, Warning, TEXT(">>>>> DamageState CurTime : %f"), CurTime);
		}
		else
		{
			EnemyState = EEnemyState::IDLE; // �̵� �߿� ���� ��� or ���� ���¿��� ���� ���
			EnemyMovement = EEnemyMovement::MAX;
			// ��� �ð� �ʱ�ȭ
			CurTime = IdleDelayTime* 0.98;
			//*** Debug
			UE_LOG(LogTemp, Warning, TEXT(">>>>> DamageState CurTime : %f"), CurTime);
		}
		

		Self->CheckSubState();

	
		// �ִϸ��̼� ���� ����ȭ
		Anim->MainAnimState = EnemyState;
		Anim->SubAnimMovement = EnemyMovement;
	}
}


void UC_EnemyFSM::DeadState()
{
	// �ִϸ��̼��� ������ ��������� �Ѵ�
	if( !bDeadDone ) return;
	
	// �ð��� �帣�ٰ�
	CurTime += GetWorld()->DeltaRealTimeSeconds;
	
	// ��� �ð��� ��� �ð��� �ʰ��ϸ�
	if (CurTime > DestroyDelayTime)
	{
		// Enemy�� ���Ž�Ų��
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
	
	/*** �Ÿ��� ���� �ٰŸ� ���� ���·� ��ȯ ***/
	CanMeleeAttack(dir.Size());
}


void UC_EnemyFSM::CanMeleeAttack(float InDistance)
{
	/*** �Ÿ��� ���� �ٰŸ� ���� ���·� ��ȯ ***/
	if (InDistance < MeleeRange)
	{
		// �������� �����
		MyAI->StopMovement();

		// ���� ���·� ��ȯ
		EnemyState = EEnemyState::ATTACK;
		EnemyMovement = EEnemyMovement::MAX;

		// �ִϸ��̼� ���� ����ȭ
		Anim->MainAnimState = EnemyState;
		Anim->SubAnimMovement = EnemyMovement;

		// ���� �ִϸ��̼� ��� Ȱ��ȭ
		Anim->bAttackPlay = true;

		// ���� ���� ��ȯ �� ��� �ð� ���� �ٷ� �÷��� �ǵ���
		CurTime = AttackDelayTime;
	}
}

// Animation Montage�� �����ϴ� �κ��� ���Ƽ� �Լ��� ����
void UC_EnemyFSM::PlayEnemyMontage(FString* InSectionName)
{
	if (InSectionName->Contains(TEXT("Damage")))
	{
		// Damage Animation Montage ����
		Self->PlayAnimMontage(Anim->EnemyMontage, 2.f, FName(*InSectionName));
	}
	else
	{
		// Dead Animation Montage ����
		Self->PlayAnimMontage(Anim->EnemyMontage, 3.f, FName(*InSectionName));
	}
}

void UC_EnemyFSM::CanMove(float InDistance)
{
	// ���� Player�� �ٰŸ� �������� ����ٸ�
	if (InDistance > MeleeRange)
	{
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

// Enemy Main state �� Movement state ����
void UC_EnemyFSM::ChangeEnemyStates(EEnemyState InMain, EEnemyMovement InMovement)
{
	EnemyState = InMain;
	EnemyMovement = InMovement;
}

// Enemy Movement ���¿� ���� Damage Animation Montage ���
void UC_EnemyFSM::PlayDamageAM(FString* InSectionName)
{
	if (EnemyMovement == EEnemyMovement::WALK)
		PlayEnemyMontage(InSectionName);
}
