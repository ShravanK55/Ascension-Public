// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameAbilitySystemComponent.h"

#include "PlayerAbilitySystemGlobals.h"

#include "PlayerAbilitySystemComponent.generated.h"

/*
 * Class for the player ability system component. 
 */
UCLASS()
class ASCENSION_API UPlayerAbilitySystemComponent : public UGameAbilitySystemComponent
{
    GENERATED_BODY()
    
public:
    // Sets default values for this component's properties
    UPlayerAbilitySystemComponent();

    /*
     * Function to get the ability name from an identifier.
     * @param AbilityIdentifier		Identifier of the ability.
     */
    UFUNCTION(BlueprintCallable, Category = "Player Abilities")
    static FString GetAbilityName(EPlayerAbility AbilityIdentifier);
    
};
