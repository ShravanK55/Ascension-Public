// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStateComponent.generated.h"


UENUM(BlueprintType)
enum class ECharacterState : uint8
{
    CS_Idle             UMETA(DisplayName = "Idle"),
    CS_Switching        UMETA(DisplayName = "Switching"),
    CS_Dodging          UMETA(DisplayName = "Dodging"),
    CS_Attacking        UMETA(DisplayName = "Attacking"),
    CS_Stunned          UMETA(DisplayName = "Stunned"),
    CS_Dead             UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
    WS_Sheathed        UMETA(DisplayName = "Sheathed"),
    WS_Unsheathed      UMETA(DisplayName = "Unsheathed")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASCENSION_API UPlayerStateComponent : public UActorComponent
{
    GENERATED_BODY()

public:	
    // Sets default values for this component's properties
    UPlayerStateComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

protected:
    /** General state that the character is in. */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character State")
    ECharacterState CharacterState;

    /** State that the character's weapon is in. Determines whether the weapon is sheathed/unsheathed. */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character State")
    EWeaponState WeaponState;

public:
    /*
     * Gets the character state.
     * @returns ECharacterState		State of the character.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character State")
    ECharacterState GetCharacterState() const;

    /*
     * Gets the movement mode.
     * @returns EMovementMode		State of the character's movement.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character State")
    EMovementMode GetMovementMode() const;

    /*
     * Gets the weapon state.
     * @returns EWeaponState		State of the character's weapon.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character State")
    EWeaponState GetWeaponState() const;

    /*
     * Sets the character state.
     * @param State		State of the character.
     */
    UFUNCTION(BlueprintCallable, Category = "State Helper")
    void SetCharacterState(ECharacterState State);

    /*
     * Sets the weapon state.
     * @param State		State of the character's weapon.
     */
    UFUNCTION(BlueprintCallable, Category = "State Helper")
    void SetWeaponState(EWeaponState State);

protected:
    /** The component's owner. */
    UPROPERTY(VisibleAnywhere, Category = "Owner")
    AActor* Owner;
};
