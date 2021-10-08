// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CombatComponent.h"

#include "Components/Player/PlayerAbilitySystemGlobals.h"

#include "PlayerCombatComponent.generated.h"


/*
 * Namespace for the different types of attacks the user can perform.
 */
namespace ActionTypes
{
    const static FString LIGHT_ATTACK = FString("Light Attack");
    const static FString SPECIAL_ATTACK = FString("Special Attack");
    const static FString STATIC_LAUNCH_ATTACK = FString("Static Launch Attack");
    const static FString AERIAL_LAUNCH_ATTACK = FString("Aerial Launch Attack");
    const static FString STINGER = FString("Stinger");
    const static FString BASE_DODGE = FString("Dodge");
}


/*
 * Component handling combat actions for the player.
 */
UCLASS()
class ASCENSION_API UPlayerCombatComponent : public UCombatComponent
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    EChainState ChainState;

public:
    // Sets default values for this component's properties
    UPlayerCombatComponent();

    /*
     * Method to get the combat action name for a given input action.
     * @param InputAction    Input action to get the name for.
     * @returns FString      Name of the combat action.
     */
    UFUNCTION(BlueprintCallable, Category = "Attacks")
    FString GetActionName(EPlayerInputAction InputAction) const;

    /*
     * Method to get the perform an action of a given type.
     * @param InputAction    Input action to get the name for.
     * @returns bool         Whether the action was performed.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    bool PerformAction(EPlayerInputAction InputAction);
    virtual bool PerformAction_Implementation(EPlayerInputAction InputAction);

    /*
     * Method to select an attack for a given type.
     * @param AttackType    Type of attack to perform.
     * @returns FString     Name of attack ability to perform.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    FString SelectAttack(const FString& AttackType);
    virtual FString SelectAttack_Implementation(const FString& AttackType);

    /*
     * Called for the character to setup the specified attack.
     * @param AttackName    Name of the attack to setup.
     */
    virtual void SetupAttack_Implementation(const FString& AttackName);

    /*
     * Called for the character to perform the specified attack.
     * @param AttackName    Name of the attack to perform.
     * @returns bool        Whether the attack was executed.
     */
    virtual bool Attack_Implementation(const FString& AttackName);

    /*
     * Called for the character to finish the specified attack.
     * @param AttackName    Name of the attack to finish.
     */
    virtual void FinishAttack_Implementation(const FString& AttackName);

    /*
     * Function to get the chain state.
     * @returns EChainState    Chain state of the component.
     */
    UFUNCTION(BlueprintCallable, Category = "Attacks")
    EChainState GetChainState() const;

    /*
     * Function to set the chain state.
     * @param ChainState    Chain state to set
     */
    UFUNCTION(BlueprintCallable, Category = "Attacks")
    void SetChainState(EChainState NewChainState);

    /*
     * Implementation for selecting dodges.
     * @param DodgeType		Type of dodge to perform.
     * @returns FString		Name of dodge ability to perform.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    FString SelectDodge(const FString& DodgeType);
    virtual FString SelectDodge_Implementation(const FString& DodgeType);

    /*
     * Called for the character to setup the dodge.
     * @param DodgeName		Name of the dodge to setup.
     */
    virtual void SetupDodge_Implementation(const FString& DodgeName);

    /*
     * Called for the character to perform the specified dodge.
     * @param DodgeName		Name of the dodge to perform.
     * @returns bool		Whether the dodge was executed.
     */
    virtual bool Dodge_Implementation(const FString& DodgeName);

    /*
     * Called for the character to finish the dodge.
     * @param DodgeName		Name of the dodge to finish.
     */
    virtual void FinishDodge_Implementation(const FString& DodgeName);

};
