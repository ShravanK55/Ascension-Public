// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "PlayerDodge.h"

#include "Components/Player/PlayerAbilitySystemGlobals.h"
#include "Components/Player/PlayerCombatComponent.h"
#include "Components/Player/PlayerStateComponent.h"


bool UPlayerDodge::CanActivate() const
{
    UPlayerCombatComponent* CombatComponent = Owner->FindComponentByClass<UPlayerCombatComponent>();
    UPlayerStateComponent* StateComponent = Owner->FindComponentByClass<UPlayerStateComponent>();

    if (StateComponent && CombatComponent)
    {
        if ((StateComponent->GetCharacterState() == ECharacterState::CS_Idle ||
             StateComponent->GetCharacterState() == ECharacterState::CS_Attacking ||
             StateComponent->GetCharacterState() == ECharacterState::CS_Dodging) &&
            (StateComponent->GetMovementMode() == EMovementMode::MOVE_Walking))
        {
            if (StateComponent->GetCharacterState() == ECharacterState::CS_Attacking ||
                StateComponent->GetCharacterState() == ECharacterState::CS_Dodging)
            {
                if (CombatComponent->GetChainState() != EChainState::ECS_None)
                {
                    return true;
                }
            }
            else
            {
                return true;
            }
        }
    }

    return false;
}
