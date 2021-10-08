// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Globals.h"
#include "Ability.generated.h"

/*
 * Class for implementing abilities which can be performed by entities.
 */
UCLASS(Blueprintable, ClassGroup = (Abilities))
class ASCENSION_API UAbility : public UObject
{
    GENERATED_BODY()
    
public:
    /*
     * Constructor of the ability.
     */
    UAbility();

public:
    /*
     * Name of the ability.
     */
    UPROPERTY(Category = Properties, EditDefaultsOnly, BlueprintReadWrite)
    FString AbilityName;

protected:
    /*
     * Movement parameters for the ability.
     * TODO: Think about refactoring this.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    FCustomMovementParams MovementParams;

public:
    /*
     * Used to initialize the ability.
     * @param Name            Name of the ability.
     * @param AbilityOwner    Entity that is to perform this ability.
     * @param System          Ability system component which handles this ability.
     */
    UFUNCTION(BlueprintCallable, Category = "Interface Functions")
    virtual void Initialize(FString Name, AActor* AbilityOwner, class UGameAbilitySystemComponent* System);

    /*
     * Used to check whether the ability can be activated.
     * @returns bool    Whether this ability can be activated.
     */
    UFUNCTION(BlueprintCallable, Category = "Interface Functions")
    virtual bool CanActivate() const;

    /*
     * This method activates the ability.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Abilities")
    void Activate();
    virtual void Activate_Implementation();

    /*
     * Tick function of an ability.
     * @param DeltaSeconds    Time elapsed since the last tick.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Abilities")
    void Tick(float DeltaSeconds);
    virtual void Tick_Implementation(float DeltaSeconds);

    /*
     * This method finishes the execution of ability.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Abilities")
    void Finish();
    virtual void Finish_Implementation();

    /*
     * Function to get the movement direction of the ability.
     * TODO: Think about refactoring this.
     * @returns FCustomMovementParams    Movement parameters of the ability.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
    FCustomMovementParams GetMovementParams() const;
    virtual FCustomMovementParams GetMovementParams_Implementation() const;

protected:
    /*
     * Entity performing the ability.
     */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Properties)
    class AActor* Owner;

    // TODO: Find a better way to initialize this.
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Properties)
    class UGameAbilitySystemComponent* AbilitySystem;
};
