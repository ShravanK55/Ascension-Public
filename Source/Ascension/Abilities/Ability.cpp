// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Ability.h"
#include "Components/GameAbilitySystemComponent.h"


UAbility::UAbility()
{
    AbilityName = FString("Ability");
    AbilitySystem = nullptr;
}

void UAbility::Initialize(FString Name, AActor* AbilityOwner = nullptr, UGameAbilitySystemComponent* System = nullptr)
{
    AbilityName = Name;
    Owner = AbilityOwner;
    AbilitySystem = System;
}

bool UAbility::CanActivate() const
{
    return false;
}

void UAbility::Activate_Implementation() {}

void UAbility::Tick_Implementation(float DeltaSeconds) {}

void UAbility::Finish_Implementation() {}

FCustomMovementParams UAbility::GetMovementParams_Implementation() const
{
    return MovementParams;
}
