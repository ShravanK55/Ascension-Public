// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Components/GameAbilitySystemComponent.h"
#include "DodgeComponent.h"


// Sets default values for this component's properties
UDodgeComponent::UDodgeComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame. You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UDodgeComponent::BeginPlay()
{
    Super::BeginPlay();

    Owner = Cast<ACharacter>(GetOwner());
}

void UDodgeComponent::SetupDodge_Implementation(const FString& DodgeName = FString("Dodge")) {}

bool UDodgeComponent::Dodge_Implementation(const FString& DodgeName = FString("Dodge"))
{
    UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

    if (AbilitySystem)
    {
        return AbilitySystem->ActivateAbility(DodgeName);
    }

    return false;
}

void UDodgeComponent::FinishDodge_Implementation(const FString& DodgeName = FString("Dodge"))
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
