// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/AttackGlobals.h"
#include "Damageable.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
    GENERATED_UINTERFACE_BODY()
};

/*
 * Interface that specifies whether an entity can be damaged.
 */
class ASCENSION_API IDamageable
{
    GENERATED_IINTERFACE_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /** Function return the entity's health percentage between 0 and 1. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
    void GetHealthPercent(float& HealthPercent);

    /** Applies the effects of the hit onto the entity. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
    void ApplyHitEffect(const AActor* SourceActor, const float Damage, const EHitEffect HitEffect, const FAttackEffect AttackEffect);

    /** Displays the entity's health bar. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
    void ShowHealthBar();

    /** Hides the entity's health bar. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
    void HideHealthBar();

    /** Checks if the entity is dead. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
    bool IsDead();
};
