// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Entities/GameCharacter.h"

#include "Enemy.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRecovered);


UCLASS()
class ASCENSION_API AEnemy : public AGameCharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AEnemy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    /** Event fired when enemy is hit. */
    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatchers")
    FOnHit OnHit;

    /** Event fired when enemy has recovered from a hit. */
    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatchers")
    FOnRecovered OnRecovered;

public:
    /** Function to use an attack. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void Attack();
    virtual void Attack_Implementation();

public:	
    /** Variable that indicates whether the entity is dead/alive. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Parameters")
    bool Dead;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
