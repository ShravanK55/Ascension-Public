// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameMovementComponent.generated.h"


/*
 * Component handling movement of entities in the game.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASCENSION_API UGameMovementComponent : public UCharacterMovementComponent
{
    GENERATED_BODY()

public:	
    // Sets default values for this component's properties
    UGameMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
    /** Normal speed of the character. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    float DefaultSpeed;

    /** Acceleration of the character. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    float DefaultAcceleration;

    /** Base turn rate of the character. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    float DefaultTurnRate;

    /** Whether we want the rotation of the character to match the direction of movement. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    bool DefaultOrientRotationToMovement;

    /** Direction the entity wants to move in. */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
    FVector MovementDirection;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    /*
     * Function to setup variables for controlled movement.
     * @param TargetSpeed                   Speed at which movement is to be performed.
     * @param TargetAcceleration            Acceleration with which movement is to be performed.
     * @param TargetTurnRate                Turn rate at which movement is to be performed.
     * @param MaxTurnAngleDegrees           Maximum angle at which the movement can differ from the character's current direction.
     */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual void SetupControlledMovement(float TargetSpeed, float TargetAcceleration, float TargetTurnRate,
                                         float MaxTurnAngleDegrees);

    /*
     * Function to setup variables for controlled movement during an ability.
     * @param AbilityName    Name of the active ability.
     */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual void SetupControlledMovementAbility(FString AbilityName);

    /*
     * Function to perform controlled movement.
     * @param MovementVector    Direction with which the entity should move.
     * @param DeltaTime         Time since the last movement update.
     * @param SetVelocity       Whether to directly set the velocity as opposed to using AddMovementInput.
     */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ControlledMove(FVector MovementVector, float DeltaTime, bool SetVelocity);

    /*
     * Function that resets movement params to default values when controlled movement completes.
     */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual void FinishControlledMovement();

protected:
    /** Called to limit character movement to a certain speed. */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetMovementSpeed(float Speed);

    /** Called to reset character movement to normal speed. */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ResetMovementSpeed();

    /** Called to set acceleration to a value. */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetAcceleration(float TargetAcceleration);

    /** Called to reset acceleration. */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ResetAcceleration();

    /** Called to limit character turning to a certain rate. */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetTurningRate(float Rate);

    /** Called to reset character turning to normal rate. */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ResetTurningRate();

    /** Sets the direction for the entity to move in. */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetMovementDirection(FVector Direction);

    /** Sets movement mode to flying. */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetFlyable();

    /** Sets movement mode to walking. */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ResetFlyable();

    /** Called to set gravity to a value. */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetGravity(float GravityValue);

    /** Called to reset gravity. */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ResetGravity();

};
