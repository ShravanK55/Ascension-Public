// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "PlayerCombatComponent.h"

#include "Components/Player/PlayerAbilitySystemComponent.h"
#include "Components/Player/PlayerStateComponent.h"


UPlayerCombatComponent::UPlayerCombatComponent()
{
    ChainState = EChainState::ECS_None;
}


FString UPlayerCombatComponent::GetActionName(EPlayerInputAction InputAction) const
{
    switch (InputAction)
    {
    case EPlayerInputAction::EPIA_LightAttack:
        return ActionTypes::LIGHT_ATTACK;

    case EPlayerInputAction::EPIA_StaticLaunch:
        return ActionTypes::STATIC_LAUNCH_ATTACK;

    case EPlayerInputAction::EPIA_AerialLaunch:
        return ActionTypes::AERIAL_LAUNCH_ATTACK;

    case EPlayerInputAction::EPIA_Stinger:
        return ActionTypes::STINGER;

    case EPlayerInputAction::EPIA_Dodge:
        return ActionTypes::BASE_DODGE;

    default:
        return FString("");
    }
}


bool UPlayerCombatComponent::PerformAction_Implementation(EPlayerInputAction InputAction)
{
    switch (InputAction)
    {
    case EPlayerInputAction::EPIA_LightAttack:
    case EPlayerInputAction::EPIA_StaticLaunch:
    case EPlayerInputAction::EPIA_AerialLaunch:
    case EPlayerInputAction::EPIA_Stinger:
        return Attack(GetActionName(InputAction));

    case EPlayerInputAction::EPIA_Dodge:
        return Dodge(GetActionName(InputAction));

    default:
        return false;
    }
}


FString UPlayerCombatComponent::SelectAttack_Implementation(const FString& AttackType)
{
    FString AttackName = FString();
    UPlayerStateComponent* StateComponent = Owner->FindComponentByClass<UPlayerStateComponent>();
    UPlayerAbilitySystemComponent* AbilitySystemComponrnt = Owner->FindComponentByClass<UPlayerAbilitySystemComponent>();

    if (StateComponent && AbilitySystemComponrnt)
    {
        EMovementMode MovementMode = StateComponent->GetMovementMode();
        TArray<FString> ActiveAbilities = AbilitySystemComponrnt->GetActiveAbilityNames();

        switch (MovementMode)
        {
        case EMovementMode::MOVE_Walking:
            if (AttackType.Equals(ActionTypes::LIGHT_ATTACK))
            {
                if (ChainState == EChainState::ECS_ComboB)
                {
                    if (ActiveAbilities.Contains(UPlayerAbilitySystemComponent::GetAbilityName(EPlayerAbility::EPA_Light03B)))
                    {
                        AttackName = UPlayerAbilitySystemComponent::GetAbilityName(EPlayerAbility::EPA_Light04B);
                    }
                    else
                    {
                        AttackName = UPlayerAbilitySystemComponent::GetAbilityName(EPlayerAbility::EPA_Light03B);
                    }
                }

                else
                {
                    if (ActiveAbilities.Contains(UPlayerAbilitySystemComponent::GetAbilityName(EPlayerAbility::EPA_Light01)))
                    {
                        AttackName = UPlayerAbilitySystemComponent::GetAbilityName(EPlayerAbility::EPA_Light02);
                    }
                    else if (ActiveAbilities.Contains(UPlayerAbilitySystemComponent::GetAbilityName(EPlayerAbility::EPA_Light02)))
                    {
                        AttackName = UPlayerAbilitySystemComponent::GetAbilityName(EPlayerAbility::EPA_Light03A);
                    }
                    else
                    {
                        AttackName = UPlayerAbilitySystemComponent::GetAbilityName(EPlayerAbility::EPA_Light01);
                    }
                }
            }

            else if (AttackType.Equals(ActionTypes::STATIC_LAUNCH_ATTACK))
            {
                AttackName = UPlayerAbilitySystemComponent::GetAbilityName(EPlayerAbility::EPA_StaticLaunch);
            }

            else if (AttackType.Equals(ActionTypes::AERIAL_LAUNCH_ATTACK))
            {
                AttackName = UPlayerAbilitySystemComponent::GetAbilityName(EPlayerAbility::EPA_AerialLaunch);
            }

            else if (AttackType.Equals(ActionTypes::STINGER))
            {
                AttackName = UPlayerAbilitySystemComponent::GetAbilityName(EPlayerAbility::EPA_Stinger);
            }

            break;

        case EMovementMode::MOVE_Flying:
        case EMovementMode::MOVE_Falling:
            if (AttackType.Equals(ActionTypes::LIGHT_ATTACK))
            {
                if (ActiveAbilities.Contains(UPlayerAbilitySystemComponent::GetAbilityName(EPlayerAbility::EPA_AirLight01)))
                {
                    AttackName = UPlayerAbilitySystemComponent::GetAbilityName(EPlayerAbility::EPA_AirLight02);
                }
                else if (ActiveAbilities.Contains(UPlayerAbilitySystemComponent::GetAbilityName(EPlayerAbility::EPA_AirLight02)))
                {
                    AttackName = UPlayerAbilitySystemComponent::GetAbilityName(EPlayerAbility::EPA_AirLight03);
                }
                else if (ActiveAbilities.Contains(UPlayerAbilitySystemComponent::GetAbilityName(EPlayerAbility::EPA_AirLight03)))
                {
                    AttackName = UPlayerAbilitySystemComponent::GetAbilityName(EPlayerAbility::EPA_AirLight04);
                }
                else
                {
                    AttackName = UPlayerAbilitySystemComponent::GetAbilityName(EPlayerAbility::EPA_AirLight01);
                }
            }

            else if (AttackType.Equals(ActionTypes::STATIC_LAUNCH_ATTACK) || AttackType.Equals(ActionTypes::AERIAL_LAUNCH_ATTACK))
            {
                AttackName = UPlayerAbilitySystemComponent::GetAbilityName(EPlayerAbility::EPA_AirSlam);
            }

            break;

        default:
            break;
        }
    }

    return AttackName;
}


void UPlayerCombatComponent::SetupAttack_Implementation(const FString& AttackName)
{
    UPlayerStateComponent* StateComponent = Owner->FindComponentByClass<UPlayerStateComponent>();

    if (StateComponent != nullptr)
    {
        StateComponent->SetCharacterState(ECharacterState::CS_Attacking);
    }
}


bool UPlayerCombatComponent::Attack_Implementation(const FString& AttackName)
{
    // This is done to choose the correct attack in a combo.
    FString PlayerAttackName = SelectAttack(AttackName);

    UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();
    if (AbilitySystem)
    {
        return AbilitySystem->ActivateAbility(PlayerAttackName);
    }

    return false;
}


void UPlayerCombatComponent::FinishAttack_Implementation(const FString& AttackName)
{
    UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

    if (!AttackName.Equals(FString("")))
    {
        if (AbilitySystem)
        {
            AbilitySystem->FinishAbility(AttackName);
        }

        UPlayerStateComponent* StateComponent = Owner->FindComponentByClass<UPlayerStateComponent>();
        if (StateComponent)
        {
            StateComponent->SetCharacterState(ECharacterState::CS_Idle);
        }
    }
}


EChainState UPlayerCombatComponent::GetChainState() const
{
    return ChainState;
}


void UPlayerCombatComponent::SetChainState(EChainState NewChainState)
{
    this->ChainState = NewChainState;
}


FString UPlayerCombatComponent::SelectDodge_Implementation(const FString& DodgeType)
{
    // TODO: Implement direction and lock-on based dodges/steps.
    return UPlayerAbilitySystemComponent::GetAbilityName(EPlayerAbility::EPA_Dodge);
}


void UPlayerCombatComponent::SetupDodge_Implementation(const FString& DodgeName = ActionTypes::BASE_DODGE)
{
    UPlayerStateComponent* StateComponent = Owner->FindComponentByClass<UPlayerStateComponent>();
    if (StateComponent != nullptr)
    {
        StateComponent->SetCharacterState(ECharacterState::CS_Dodging);
    }
}


bool UPlayerCombatComponent::Dodge_Implementation(const FString& DodgeName = ActionTypes::BASE_DODGE)
{
    // This is done to choose the correct dodge.
    FString PlayerDodgeName = SelectDodge(DodgeName);

    return Super::Dodge_Implementation(PlayerDodgeName);
}


void UPlayerCombatComponent::FinishDodge_Implementation(const FString& DodgeName = ActionTypes::BASE_DODGE)
{
    UGameAbilitySystemComponent* AbilitySystem = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

    if (!DodgeName.Equals(FString("")))
    {
        if (AbilitySystem)
        {
            AbilitySystem->FinishAbility(DodgeName);
        }

        UPlayerStateComponent* StateComponent = Owner->FindComponentByClass<UPlayerStateComponent>();
        if (StateComponent)
        {
            StateComponent->SetCharacterState(ECharacterState::CS_Idle);
        }
    }
}
