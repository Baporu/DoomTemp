#include "Enemy/C_EnemyFSM.h"
#include "C_PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/C_EnemyA.h"
#include "AIController.h"

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
		//LongRange = Self->GetLongRange();
		// Speed = Self->GetSpeed();

	}
	

	/***** AI Controller *****/
	Ai = Cast<AAIController>(Self->GetController());
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
	//if (Self->bIsFlinched == true)
	//{
	//	// 1. �Ͻ������� �������� ��������
	//
	//	// 2. ���� ������ ��¦�δ�
	//}


	/***** �̵� *****/
	// 1. Target ������ ���Ѵ�
	FVector dir = Target->GetActorLocation() - Self->GetActorLocation();

	// 2. Target ��ġ�� �̵��Ѵ�
	//Self->AddMovementInput( dir.GetSafeNormal() );
	Ai->MoveToLocation( Target->GetActorLocation() );


	/***** �ٰŸ� ���� ���·� ��ȯ *****/
	// 1. ���� Player�� �ٰŸ� ���� ���� �ȿ� ������
	if ( dir.Size() <= MeleeRange )
	{
		// 2. ���� ���·� ��ȯ
		EnemyState = EEnemyState::ATTACK;
	}
}

void UC_EnemyFSM::AttackState()
{
	float distance = FVector::Distance( Target->GetActorLocation(), Self->GetActorLocation() );
	
	// 1. �ð��� �帥��
	CurTime += GetWorld()->DeltaTimeSeconds;
	
	// 2. ���� ��� �ð��� ��� �ð��� �ʰ��ϸ�
	if (CurTime > AttackDelayTime)
	{
		// 3. �ٰŸ� ������ �Ѵ�
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, L"Long Range Attack!!!!!");

        //// 4. ���Ÿ� ������ �Ѵ�
        //GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, L"Melee Attack!!!!!");

		CurTime = 0.f;
	}
	
	// 5-1. ���� Player�� �ٰŸ� �������� ����ٸ�
	if (distance > MeleeRange )
	{
		// 5-2. ���¸� �̵����� ��ȯ�Ѵ�
		EnemyState = EEnemyState::MOVE;
	}
}

void UC_EnemyFSM::DamageState()
{

}

void UC_EnemyFSM::DeadState()
{

}

