// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "PlayerStateComponent.h"

#include "GameFramework/CharacterMovementComponent.h"


// Sets default values for this component's properties
UPlayerStateComponent::UPlayerStateComponent()
{
    // Set character states.
    CharacterState = ECharacterState::CS_Idle;
    WeaponState = EWeaponState::WS_Sheathed;
}

// Called when the game starts
void UPlayerStateComponent::BeginPlay()
{
    Super::BeginPlay();

    Owner = GetOwner();
}

ECharacterState UPlayerStateComponent::GetCharacterState() const
{
    return CharacterState;
}

EMovementMode UPlayerStateComponent::GetMovementMode() const
{
    UCharacterMovementComponent* MovementComponent = Owner->FindComponentByClass<UCharacterMovementComponent>();

    if (MovementComponent)
    {
        return MovementComponent->MovementMode;
    }

    return EMovementMode::MOVE_None;
}

EWeaponState UPlayerStateComponent::GetWeaponState() const
{
    return WeaponState;
}

void UPlayerStateComponent::SetCharacterState(ECharacterState State)
{
    CharacterState = State;
}

void UPlayerStateComponent::SetWeaponState(EWeaponState State)
{
    WeaponState = State;
}
