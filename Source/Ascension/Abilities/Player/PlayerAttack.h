// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/AnimAbility.h"
#include "PlayerAttack.generated.h"

/*
 * Class for player attacks.
 */
UCLASS()
class ASCENSION_API UPlayerAttack : public UAnimAbility
{
    GENERATED_BODY()

public:
    /*
     * Used to check whether the ability can be activated.
     * @returns bool    Whether this ability can be activated.
     */
    virtual bool CanActivate() const override;

};
