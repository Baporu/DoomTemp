#include "Enemy/C_EnemyManager.h"
#include "Enemy/C_Enemy.h"


AC_EnemyManager::AC_EnemyManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AC_EnemyManager::BeginPlay()
{
	Super::BeginPlay();

	/***** Timer *****/
	// 게임이 시작되고 첫 스폰은 5초 후에 시작되도록
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AC_EnemyManager::CreateEnemy, 4.5f);

	isGameStarted = true;
}

void AC_EnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AC_EnemyManager::CreateEnemy()
{
	// 랜덤 스폰 위치
	int32 index;

	// 게임 처음 시작될 때에는 무조건 1번 SpawnPoint에서 spawn됨
	if (isGameStarted)
	{
		index = 1;
		isGameStarted = false;
	}
	else
		index = FMath::RandRange(0, SpawnPoints.Num() - 1);

	// Enemy Spawn
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AC_Enemy>(EnemyFactory, SpawnPoints[index]->GetActorLocation(), FRotator(0), param);

	// 새로운 랜덤 시간
	float createTime = FMath::RandRange(MinTime, MaxTime);

	// 다시 타이머에 등록
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AC_EnemyManager::CreateEnemy, createTime);
}
