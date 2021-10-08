// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/AnimAbility.h"
#include "PlayerDodge.generated.h"

/*
 * Class for player dodge.
 */
UCLASS()
class ASCENSION_API UPlayerDodge : public UAnimAbility
{
	GENERATED_BODY()

public:
    /*
     * Used to check whether the ability can be activated.
     * @returns bool    Whether this ability can be activated.
     */
    virtual bool CanActivate() const override;

};
