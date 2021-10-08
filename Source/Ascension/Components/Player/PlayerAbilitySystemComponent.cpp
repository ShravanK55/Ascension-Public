// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "PlayerAbilitySystemComponent.h"


// Sets default values for this component's properties
UPlayerAbilitySystemComponent::UPlayerAbilitySystemComponent() {}

FString UPlayerAbilitySystemComponent::GetAbilityName(EPlayerAbility AbilityIdentifier)
{
    return ABILITY_ID_NAME_MAP[AbilityIdentifier];
}
