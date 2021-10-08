// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Globals.h"
#include "AttackComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackComplete, bool, Successful);


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASCENSION_API UAttackComponent : public UActorComponent
{
    GENERATED_BODY()

public:	
    // Sets default values for this component's properties
    UAttackComponent();
    
    /*
     * Function called to initialize the attack component.
     * To be called from main entity to initialize these variables.
     */
    UFUNCTION(BlueprintCallable, Category = "Initialization")
    void Initialize();

protected:
    // Called when the component is in play.
    virtual void BeginPlay() override;

    // Called when the component exits play.
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    
protected:
    /** Collision box that checks what actors are hit. */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
    UBoxComponent* AttackHitBox;

    /*
     * Array containing actors that have been damaged by this attack. 
     * TODO: Move to Ability.
     */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Damage")
    TArray<AActor*> DamagedActors;

public:
    /** Event that is broadcast when an attack succeeds/fails. */
    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatchers")
    FOnAttackComplete OnAttackComplete;

protected:
    /** The direction the character should perform an action. */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
    FVector ActionDirection;

public:
    /*
     * Called for the character to setup the specified attack.
     * @param AttackName	Name of the attack to setup.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
    void SetupAttack(const FString& AttackName);
    virtual void SetupAttack_Implementation(const FString& AttackName);

    /*
     * Called for the character to perform the specified attack.
     * @param AttackName	Name of the attack to perform.
     * @returns bool		Whether the attack was executed.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
    bool Attack(const FString& AttackName);
    virtual bool Attack_Implementation(const FString& AttackName);

    /*
     * Called for the character to finish the specified attack.
     * @param AttackName	Name of the attack to finish.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
    void FinishAttack(const FString& AttackName);
    virtual void FinishAttack_Implementation(const FString& AttackName);

    /** Scans and detects if the attack hits. */
    UFUNCTION(BlueprintCallable, Category = "Damage")
    void DetectHit();

    /** Clears the actors that have been damaged by the attack. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
    void ClearDamagedActors();

public:
    /** Finalizes character's attack direction. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void FinalizeAttackDirection(FVector MovementIntent);

protected:
    /** The component's owner. */
    UPROPERTY(VisibleAnywhere, Category = "Owner")
    ACharacter* Owner;

};
