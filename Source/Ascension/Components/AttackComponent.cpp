// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "AttackComponent.h"
#include "GameMovementComponent.h"
#include "Interfaces/Damageable.h"
#include "Components/GameAbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
    // Set this component to be initialized when the game starts, and to not be ticked every frame.
    PrimaryComponentTick.bCanEverTick = true;

    // Set gameplay variables.
    AttackHitBox = nullptr;
    DamagedActors.Empty();
}


void UAttackComponent::Initialize()
{
}

// Called when the actor is in play.
void UAttackComponent::BeginPlay()
{
    Super::BeginPlay();

    Owner = Cast<ACharacter>(GetOwner());
}

void UAttackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

void UAttackComponent::SetupAttack_Implementation(const FString& AttackName = FString("")) {}

bool UAttackComponent::Attack_Implementation(const FString& AttackName)
{
    UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

    if (AbilitySystem)
    {
        return AbilitySystem->ActivateAbility(AttackName);
    }

    return false;
}

void UAttackComponent::FinishAttack_Implementation(const FString& AttackName = FString(""))
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

void UAttackComponent::DetectHit()
{
    // TODO: Rework this.
    /*
    if (AttackHitBox)
    {
        TArray<UPrimitiveComponent*> OverlappingComponents;
        AttackHitBox->GetOverlappingComponents(OverlappingComponents);

        for (int i = 0; i < OverlappingComponents.Num(); i++)
        {
            UPrimitiveComponent* OtherComponent = OverlappingComponents[i];
            if (OtherComponent->ComponentHasTag(FName("Hitbox")))
            {
                AActor* OtherActor = OtherComponent->GetOwner();

                if (DamagedActors.Find(OtherActor) == INDEX_NONE && OtherActor != GetOwner())
                {
                    if (OtherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
                    {
                        UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

                        if (!(ActiveAttacks.Num() == 0) && AbilitySystem != nullptr)
                        {
                            // Get the correct attack, not the zeroth index.
                            UAttack* Attack = Cast<UAttack>(AbilitySystem->GetAbility(ActiveAttacks[0]));

                            if (Attack != nullptr)
                            {
                                IDamageable::Execute_ApplyHitEffect(OtherActor, GetOwner(), 
                                                                    Attack->GetEffectInfo().Damage, 
                                                                    Attack->GetEffectInfo().HitEffect, 
                                                                    Attack->GetEffectInfo().AttackEffect);
                                DamagedActors.Add(OtherActor);
                            }
                        }
                    }
                }
            }
        }
    }
    */
}

void UAttackComponent::ClearDamagedActors_Implementation()
{
    DamagedActors.Empty();
}

void UAttackComponent::FinalizeAttackDirection_Implementation(FVector MovementIntent)
{
    ActionDirection = MovementIntent;
}
