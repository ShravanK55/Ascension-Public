// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "PlayerAttack.h"

#include "Components/Player/PlayerAbilitySystemGlobals.h"
#include "Components/Player/PlayerCombatComponent.h"
#include "Components/Player/PlayerStateComponent.h"


bool UPlayerAttack::CanActivate() const
{
    UPlayerCombatComponent* CombatComponent = Owner->FindComponentByClass<UPlayerCombatComponent>();
    UPlayerStateComponent* StateComponent = Owner->FindComponentByClass<UPlayerStateComponent>();

    if (StateComponent)
    {
        if ((StateComponent->GetCharacterState() == ECharacterState::CS_Idle ||
            StateComponent->GetCharacterState() == ECharacterState::CS_Attacking) &&
            (StateComponent->GetWeaponState() == EWeaponState::WS_Unsheathed))
        {
            if (StateComponent->GetCharacterState() == ECharacterState::CS_Attacking)
            {
                if (CombatComponent->GetChainState() != EChainState::ECS_None)
                {
                    // TODO: This needs to be done by a anim notify sequence and not via a variable.
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
