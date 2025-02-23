// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DropBase.generated.h"

UENUM()
enum class EDropType {
	Health,
	Saw,
	Plasma,
	Sniper,
	Shotgun,
	MAX
};

UCLASS()
class DOOMTEMP_API ADropBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	// Collider Component
	UPROPERTY(VisibleAnywhere, Category = "Basics | Components")
	class UBoxComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly, Category = "Basics | Stats")
	EDropType DropType;
	UPROPERTY(EditDefaultsOnly, Category = "Basics | Stats")
	int32 HealValue;
	UPROPERTY(EditDefaultsOnly, Category = "Basics | Stats")
	float FlyTime = 1.5f;
	float FlyTimer = 0.0f;
	bool bEnabledCollision = false;

	FVector Dir;


	UFUNCTION()
	void OnDropOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
