// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "GameAbilitySystemComponent.h"
#include "UObject/UObjectGlobals.h"
#include "Abilities/Ability.h"

// Sets default values for this component's properties
UGameAbilitySystemComponent::UGameAbilitySystemComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    ClearAbilities();
    Owner = GetOwner();
}


// Called when the game starts
void UGameAbilitySystemComponent::BeginPlay()
{
    Super::BeginPlay();
}


// Called every frame
void UGameAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

TSubclassOf<UAbility> UGameAbilitySystemComponent::GetAbility(const FString& AbilityName) const
{
    if (AbilitiesMap.Contains(AbilityName))
    {
        return AbilitiesMap[AbilityName];
    }
    
    return nullptr;
}

UAbility* UGameAbilitySystemComponent::GetActiveAbility(const FString& AbilityName = FString("")) const
{
    if (!AbilityName.Equals(FString("")))
    {
        if (ActiveAbilitiesMap.Contains(AbilityName))
        {
            return ActiveAbilitiesMap[AbilityName];
        }
    }

    return nullptr;
}

TArray<UAbility*> UGameAbilitySystemComponent::GetActiveAbilities() const
{
    TArray<UAbility*> ActiveAbilitiesOut;
    ActiveAbilitiesMap.GenerateValueArray(ActiveAbilitiesOut);
    return ActiveAbilitiesOut;
}

TArray<FString> UGameAbilitySystemComponent::GetActiveAbilityNames() const
{
    TArray<FString> ActiveAbilityNamesOut;
    ActiveAbilitiesMap.GenerateKeyArray(ActiveAbilityNamesOut);
    return ActiveAbilityNamesOut;
}

void UGameAbilitySystemComponent::AddAbility(const FString& AbilityName, TSubclassOf<UAbility> Ability)
{
    if (!AbilitiesMap.Contains(AbilityName))
    {
        AbilitiesMap.Add(AbilityName, Ability);
    }
}

void UGameAbilitySystemComponent::ClearAbilities()
{
    AbilitiesMap.Empty();
    ActiveAbilitiesMap.Empty();
}

bool UGameAbilitySystemComponent::ActivateAbility(const FString& AbilityName)
{
    TSubclassOf<UAbility> AbilityClass = GetAbility(AbilityName);

    if (AbilityClass != nullptr)
    {
        UAbility* Ability = NewObject<UAbility>(this, AbilityClass);
        Ability->Initialize(AbilityName, Owner, this);
        if (Ability->CanActivate())
        {
            Ability->Activate();
            ActiveAbilitiesMap.Add(AbilityName, Ability);
            return true;
        }
    }

    return false;
}

void UGameAbilitySystemComponent::TickAbility(const FString& AbilityName = FString(""), float DeltaSeconds = 0.0f)
{
    if (!AbilityName.Equals(FString("")))
    {
        if (ActiveAbilitiesMap.Contains(AbilityName))
        {
            ActiveAbilitiesMap[AbilityName]->Tick(DeltaSeconds);
        }
    }
}

void UGameAbilitySystemComponent::FinishAbility(const FString& AbilityName = FString(""))
{
    if (!AbilityName.Equals(FString("")))
    {
        if (ActiveAbilitiesMap.Contains(AbilityName))
        {
            ActiveAbilitiesMap[AbilityName]->Finish();
            ActiveAbilitiesMap.Remove(AbilityName);
        }
    }
}

void UGameAbilitySystemComponent::PrintActiveAbilities() const
{
    FString ActiveAbilitiesString = FString("Abilities: ");

    for (auto& Pair : ActiveAbilitiesMap)
    {
        ActiveAbilitiesString = ActiveAbilitiesString.Append(Pair.Key);
        ActiveAbilitiesString = ActiveAbilitiesString.Append(FString(" | "));
    }

    UE_LOG(LogTemp, Warning, TEXT("%s"), *ActiveAbilitiesString)
}
