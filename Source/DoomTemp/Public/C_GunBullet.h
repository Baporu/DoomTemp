// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_GunBullet.generated.h"

UCLASS()
class DOOMTEMP_API AC_GunBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_GunBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float BulletSpeed = 5000.0f;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float LifeTime = 2.0f;

	UPROPERTY(VisibleAnywhere, Category = Stats)
	int32 Damage = 0;

public:
	// VFX
	UPROPERTY(EditAnywhere, Category = VFX)
	class UParticleSystem* BulletVFX;


	UFUNCTION()
	void OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnBulletInit(int32 InBulletDamage);
};
