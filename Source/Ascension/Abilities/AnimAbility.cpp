// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Abilities/AnimAbility.h"


UAnimAbility::UAnimAbility()
    : Super()
{
    AnimMontage = nullptr;
}


void UAnimAbility::Activate_Implementation()
{
    if (AnimMontage != nullptr && AbilitySystem != nullptr)
    {
        ACharacter* CharacterOwner = Cast<ACharacter>(Owner);
        CharacterOwner->PlayAnimMontage(AnimMontage);
    }
}
