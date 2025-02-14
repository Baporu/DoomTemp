// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_GunBullet.h"
#include "C_ProjectileBullet.generated.h"

/**
 * 
 */
UCLASS()
class DOOMTEMP_API AC_ProjectileBullet : public AC_GunBullet
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AC_ProjectileBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector Dir;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	float BulletSpeed = 5000.0f;

	float LifeTimer = 2.0f;


	UFUNCTION()
	void OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);
};
