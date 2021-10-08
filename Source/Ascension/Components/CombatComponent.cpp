// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Components/CombatComponent.h"
#include "Components/GameAbilitySystemComponent.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

    Owner = Cast<ACharacter>(GetOwner());
}


void UCombatComponent::SetupAttack_Implementation(const FString& AttackName = FString("")) {}


bool UCombatComponent::Attack_Implementation(const FString& AttackName)
{
    UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

    if (AbilitySystem)
    {
        return AbilitySystem->ActivateAbility(AttackName);
    }

    return false;
}


void UCombatComponent::TickAttack_Implementation(const FString& AttackName = FString(""), float DeltaSeconds = 0.0f)
{
    UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

    if (!AttackName.Equals(FString("")))
    {
        if (AbilitySystem)
        {
            AbilitySystem->TickAbility(AttackName, DeltaSeconds);
        }
    }
}


void UCombatComponent::FinishAttack_Implementation(const FString& AttackName = FString(""))
{
    UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

    if (!AttackName.Equals(FString("")))
    {
        if (AbilitySystem)
        {
            AbilitySystem->FinishAbility(AttackName);
        }
    }
}


void UCombatComponent::SetupDodge_Implementation(const FString& DodgeName = FString("Dodge")) {}


bool UCombatComponent::Dodge_Implementation(const FString& DodgeName = FString("Dodge"))
{
    UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

    if (AbilitySystem)
    {
        return AbilitySystem->ActivateAbility(DodgeName);
    }

    return false;
}


void UCombatComponent::TickDodge_Implementation(const FString& DodgeName = FString(""), float DeltaSeconds = 0.0f)
{
    UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

    if (!DodgeName.Equals(FString("")))
    {
        if (AbilitySystem)
        {
            AbilitySystem->TickAbility(DodgeName, DeltaSeconds);
        }
    }
}


void UCombatComponent::FinishDodge_Implementation(const FString& DodgeName = FString("Dodge"))
{
    UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

    if (!DodgeName.Equals(FString("")))
    {
        if (AbilitySystem)
        {
            AbilitySystem->FinishAbility(DodgeName);
        }
    }
}
