#include "Enemy/C_EnemyFSM.h"
#include "C_PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/C_EnemyA.h"

UC_EnemyFSM::UC_EnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UC_EnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	

	/***** Target & Self *****/
	auto actor = UGameplayStatics::GetActorOfClass( GetWorld(), AC_PlayerCharacter::StaticClass() );
	Target = Cast<AC_PlayerCharacter>( actor );
	Self = Cast<AC_EnemyA>( GetOwner() );


	/***** Attack Range *****/
	if (!!Self)
	{
		MeleeRange = Self->GetMeleeRange();
		LongRange = Self->GetLongRange();

		if(!!Target)
			Direction = Target->GetActorLocation() - Self->GetActorLocation();
	}
	
}


void UC_EnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ���� ���� ȭ�鿡 ���
	FString log = UEnum::GetValueAsString(EnemyState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, log);


	switch (EnemyState)
	{
		case EEnemyState::SPAWN:
			SpawnState();
			break;
		case EEnemyState::IDLE:
			IdleState();
			break;
		case EEnemyState::PATROL:
			PatrolState();
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
	}
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
	// 1. �ð��� �帥��
	CurTime += GetWorld()->DeltaTimeSeconds;

	// 2. ���� ��� �ð��� ��� �ð��� �ʰ��ϸ�
	if (CurTime > IdleDelayTime)
	{
		// 3. �̵� ���·� ��ȯ
		EnemyState = EEnemyState::MOVE;
		CurTime = 0.f;
	}
}

void UC_EnemyFSM::PatrolState()
{
	
}

void UC_EnemyFSM::MoveState()
{
	/***** �⺻ ��������, ��Ʋ�Ÿ��� �������� üũ *****/
	////////////////////// �� �κ� �ǹ� //////////////////////
	if (Self->IsFlinched() == true)
	{
		// 1. �Ͻ������� �������� ��������

		// 2. ���� ������ ��¦�δ�
	}

	/***** �̵� *****/
	// �������� �̵�
	Self->AddMovementInput( Direction.GetSafeNormal() );


	/***** ���� ���·� ��ȯ *****/
	// 1. ���� Player�� ���� ���� �ȿ� ������
	if ( Direction.Size() <= LongRange)
	{
		// 2. ���� ���·� ��ȯ
		EnemyState = EEnemyState::ATTACK;
	}
}

void UC_EnemyFSM::AttackState()
{
	// 1. �ð��� �帥��
	CurTime += GetWorld()->DeltaTimeSeconds;

	// 2. ���� ��� �ð��� ��� �ð��� �ʰ��ϸ�
	if (CurTime > AttackDelayTime)
	{
		////////////////////// �� �κ� �ǹ� //////////////////////
		// 3-1. ���� Player�� ���Ÿ��� �ִٸ�
		if ( Direction.Size() <= Self->GetLongRange())
		{
			// 3-2. ���Ÿ� ������ �Ѵ�
			GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, L"Melee Attack!!!!!");
		}


		// 4-1. ���� Player�� �ٰŸ��� �ִٸ�
		if (Direction.Size() <= Self->GetMeleeRange())
		{
			// 4-2. �ٰŸ� ������ �Ѵ�
			GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, L"Long Range Attack!!!!!");
		}
		
		CurTime = 0.f;
	}
}

void UC_EnemyFSM::DamageState()
{

}

void UC_EnemyFSM::DeadState()
{

}

