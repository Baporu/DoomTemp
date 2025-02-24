#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_EnemyManager.generated.h"

UCLASS()
class DOOMTEMP_API AC_EnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AC_EnemyManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	// 랜덤한 시간 (최소값, 최대값)
	UPROPERTY(EditAnywhere, Category = "SpawnSetting")
	float MinTime = 2.f;
	UPROPERTY(EditAnywhere, Category = "SpawnSetting")
	float MaxTime = 4.5f;

	// 게임 시작되었는지 체크
	bool isGameStarted = false;

	// 스폰될 위치 목록
	UPROPERTY(EditAnywhere, Category = "SpawnSetting")
	TArray<AActor*> SpawnPoints;

	// Enemy Spawner
	UPROPERTY(EditAnywhere, Category = "SpawnSetting")
	TSubclassOf<class AC_Enemy> EnemyFactory;

	// 스폰 타이머
	FTimerHandle SpawnTimerHandle;

	// 적 생성 함수
	void CreateEnemy();
};