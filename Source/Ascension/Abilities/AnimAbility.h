// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability.h"
#include "AnimAbility.generated.h"

/*
 * Class for implementing abilities which have animations.
 */
UCLASS()
class ASCENSION_API UAnimAbility : public UAbility
{
	GENERATED_BODY()

public:
    /*
     * Constructor of the attack.
     */
    UAnimAbility();
	
    /*
     * Method to active the ability.
     */
    virtual void Activate_Implementation();

protected:
    /** Animation montage of the ability. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* AnimMontage;

};
