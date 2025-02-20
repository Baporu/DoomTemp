// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_GunBullet.h"
#include "C_Pellet.generated.h"

/**
 * 
 */
UCLASS()
class DOOMTEMP_API AC_Pellet : public AC_GunBullet
{
	GENERATED_BODY()

public:
	AC_Pellet();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Collision Component
	UPROPERTY(VisibleAnywhere, Category = Basics)
	class USphereComponent* CollisionComp;
	// Mesh Component
	UPROPERTY(VisibleAnywhere, Category = Basics)
	class UStaticMeshComponent* MeshComp;

	FVector Dir;

	// VFX
	UPROPERTY(EditAnywhere, Category = VFX)
	class UParticleSystem* PelletVFX;


	UFUNCTION()
	void OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);

	void ApplySpread(float InSpread);
};
