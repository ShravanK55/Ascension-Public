// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Enemy.h"


// Sets default values
AEnemy::AEnemy(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Dead = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
    Super::BeginPlay();

}

void AEnemy::Attack_Implementation()
{

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

