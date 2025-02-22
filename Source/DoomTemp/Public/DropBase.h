// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_EnumManager.h"
#include "DropBase.generated.h"

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
	// 충돌체 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class UBoxComponent* CollisionComp;
	// 외관 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = BodyMesh)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly)
	EGunType DropType;

	UPROPERTY(VisibleAnywhere)
	class AC_PlayerCharacter* Player;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsChase = false;
	UPROPERTY(VisibleAnywhere)
	bool bIsFading = false;

	float DebugTimer = 2.0f;


	UFUNCTION()
	void OnDropOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnFadeOut();
};
