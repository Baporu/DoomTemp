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
	// ������ ���۵ǰ� ù ������ 5�� �Ŀ� ���۵ǵ���
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AC_EnemyManager::CreateEnemy, 4.5f);

	isGameStarted = true;
}

void AC_EnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AC_EnemyManager::CreateEnemy()
{
	// ���� ���� ��ġ
	int32 index;

	// ���� ó�� ���۵� ������ ������ 1�� SpawnPoint���� spawn��
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

	// ���ο� ���� �ð�
	float createTime = FMath::RandRange(MinTime, MaxTime);

	// �ٽ� Ÿ�̸ӿ� ���
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AC_EnemyManager::CreateEnemy, createTime);
}
