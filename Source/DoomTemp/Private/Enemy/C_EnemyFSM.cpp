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

	// 현재 상태 화면에 출력
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
	// 1. Spawn VFX를 소환한다

	// 2. Spawn 애니메이션을 재생한다

	// 3. Idle 상태로 전환한다
	EnemyState = EEnemyState::IDLE;
}

void UC_EnemyFSM::IdleState()
{
	// 1. 시간이 흐른다
	CurTime += GetWorld()->DeltaTimeSeconds;

	// 2. 만약 경과 시간이 대기 시간을 초과하면
	if (CurTime > IdleDelayTime)
	{
		// 3. 이동 상태로 전환
		EnemyState = EEnemyState::MOVE;
		CurTime = 0.f;
	}
}

void UC_EnemyFSM::PatrolState()
{
	
}

void UC_EnemyFSM::MoveState()
{
	/***** 기본 상태인지, 비틀거리는 상태인지 체크 *****/
	////////////////////// 이 부분 의문 //////////////////////
	//if (Self->bIsFlinched == true)
	//{
	//	// 1. 일시적으로 움직임이 더뎌진다
	//
	//	// 2. 몸이 빨갛게 반짝인다
	//}


	/***** 이동 *****/
	// 1. Target 방향을 구한다
	FVector dir = Target->GetActorLocation() - Self->GetActorLocation();

	// 2. Target 위치로 이동한다
	//Self->AddMovementInput( dir.GetSafeNormal() );
	Ai->MoveToLocation( Target->GetActorLocation() );


	/***** 근거리 공격 상태로 전환 *****/
	// 1. 만약 Player가 근거리 공격 범위 안에 들어오면
	if ( dir.Size() <= MeleeRange )
	{
		// 2. 공격 상태로 전환
		EnemyState = EEnemyState::ATTACK;
	}
}

void UC_EnemyFSM::AttackState()
{
	float distance = FVector::Distance( Target->GetActorLocation(), Self->GetActorLocation() );
	
	// 1. 시간이 흐른다
	CurTime += GetWorld()->DeltaTimeSeconds;
	
	// 2. 만약 경과 시간이 대기 시간을 초과하면
	if (CurTime > AttackDelayTime)
	{
		// 3. 근거리 공격을 한다
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, L"Long Range Attack!!!!!");

        //// 4. 원거리 공격을 한다
        //GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, L"Melee Attack!!!!!");

		CurTime = 0.f;
	}
	
	// 5-1. 만약 Player가 근거리 범위에서 벗어났다면
	if (distance > MeleeRange )
	{
		// 5-2. 상태를 이동으로 전환한다
		EnemyState = EEnemyState::MOVE;
	}
}

void UC_EnemyFSM::DamageState()
{

}

void UC_EnemyFSM::DeadState()
{

}

