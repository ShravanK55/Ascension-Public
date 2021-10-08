// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameAbilitySystemComponent.generated.h"


/*
 * A component used for managing abilities tied to an entity.
 */
UCLASS(Blueprintable, ClassGroup=(AbilitySystems), meta=(BlueprintSpawnableComponent) )
class ASCENSION_API UGameAbilitySystemComponent : public UActorComponent
{
    GENERATED_BODY()

public:	
    // Sets default values for this component's properties
    UGameAbilitySystemComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:	
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
    /*
     * Map of ability names to the class of the respective ability.
     */
    UPROPERTY(Category = Abilities, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    TMap<FString, TSubclassOf<UAbility>> AbilitiesMap;

    /*
     * Map of active abilities.
     */
    UPROPERTY(Category = Abilities, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    TMap<FString, UAbility*> ActiveAbilitiesMap;

    /*
     * Ability system owner.
     */
    AActor* Owner;

public:
    /*
     * Function to get an ability.
     * @param AbilityName               Name of the ability to get.
     * @returns TSubclassOf<UAbility>   Class of the ability.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Abilities")
    TSubclassOf<UAbility> GetAbility(const FString& AbilityName) const;

    /*
     * Function to get an active ability.
     * @param AbilityName    Name of the active ability to get.
     * @returns UAbility*    Active ability with the corresponding name.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Abilities")
    UAbility* GetActiveAbility(const FString& AbilityName) const;

    /*
     * Function to get all active abilities.
     * @returns TArray<UAbility*>    Array of all active abilities.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Abilities")
    TArray<UAbility*> GetActiveAbilities() const;

    /*
     * Function to get the names of all active abilities.
     * @returns TArray<FString>     Array of all active ability names.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Abilities")
    TArray<FString> GetActiveAbilityNames() const;

    /*
     * Function to add an ability to the system.
     * @param AbilityName    Name of ability to add.
     * @param Ability        Class of ability to add.
     */
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    virtual void AddAbility(const FString& AbilityName, class TSubclassOf<UAbility> Ability);

    /*
     * Function to clear all abilities in the system.
     */
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    virtual void ClearAbilities();

    /*
     * This method activates an ability.
     * @param AbilityName           Name of the ability to activate.
     * @returns bool                Whether the ability was activated.
     */
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    virtual bool ActivateAbility(const FString& AbilityName);

    /*
     * This method calls the tick function of an ability.
     * @param AbilityName           Name of the ability to tick.
     * @param DeltaSeconds          Time elapsed since the last tick.
     */
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    virtual void TickAbility(const FString& AbilityName, float DeltaSeconds);

    /*
     * This method ends the execution of the current ability.
     * @param AbilityName    Name of the ability to finish.
     */
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    virtual void FinishAbility(const FString& AbilityName);

    /*
     * Method to print active abilities and their IDs.
     */
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void PrintActiveAbilities() const;

};
