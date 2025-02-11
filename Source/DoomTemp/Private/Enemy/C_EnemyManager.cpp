// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/C_EnemyManager.h"

// Sets default values
AC_EnemyManager::AC_EnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AC_EnemyManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_EnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

