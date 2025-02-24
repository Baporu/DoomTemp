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
	// ������ �ð� (�ּҰ�, �ִ밪)
	UPROPERTY(EditAnywhere, Category = "SpawnSetting")
	float MinTime = 2.f;
	UPROPERTY(EditAnywhere, Category = "SpawnSetting")
	float MaxTime = 4.5f;

	// ���� ���۵Ǿ����� üũ
	bool isGameStarted = false;

	// ������ ��ġ ���
	UPROPERTY(EditAnywhere, Category = "SpawnSetting")
	TArray<AActor*> SpawnPoints;

	// Enemy Spawner
	UPROPERTY(EditAnywhere, Category = "SpawnSetting")
	TSubclassOf<class AC_Enemy> EnemyFactory;

	// ���� Ÿ�̸�
	FTimerHandle SpawnTimerHandle;

	// �� ���� �Լ�
	void CreateEnemy();
};