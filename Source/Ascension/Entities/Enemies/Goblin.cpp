// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "Goblin.h"

#include "Components/AttackComponent.h"
#include "Components/GameAbilitySystemComponent.h"


FName AGoblin::AttackComponentName(TEXT("AttackComponent"));
FName AGoblin::AbilitySystemComponentName(TEXT("AbilitySystemComponent"));

AGoblin::AGoblin(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Set character parameters.
    Health = 100.0f;
    MaxHealth = 100.0f;

    // Initialize character state.
    ActionState = EEnemyState::ES_Idle;
    CombatState = EEnemyCombatState::ECS_Observing;
    AIState = EAIState::AIS_Patrol;

    // Initialize components.
    AttackComponent = nullptr;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
    GetCharacterMovement()->JumpZVelocity = 600.f;
    GetCharacterMovement()->AirControl = 0.2f;
    GetCharacterMovement()->MaxWalkSpeed = 500.0f;

    // Create and initialize the Goblin's attack component.
    AttackComponent = CreateDefaultSubobject<UAttackComponent>(AGoblin::AttackComponentName);
    AttackComponent->Initialize();

    // Create and initialize the Goblin's ability system component.
    AbilitySystemComponent = CreateDefaultSubobject<UGameAbilitySystemComponent>(AGoblin::AbilitySystemComponentName);
}

void AGoblin::GetHealthPercent_Implementation(float& HealthPercent)
{
    HealthPercent = Health / MaxHealth;
}

void AGoblin::ApplyHitEffect_Implementation(const AActor* SourceActor, const float Damage, const EHitEffect HitEffect, const FAttackEffect AttackEffect)
{
}

void AGoblin::ShowHealthBar_Implementation()
{

}

void AGoblin::HideHealthBar_Implementation()
{

}

bool AGoblin::IsDead_Implementation()
{
    return Dead;
}

void AGoblin::FootstepSound_Implementation() {}

void AGoblin::EnableMovement_Implementation()
{
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AGoblin::DisableMovement_Implementation()
{
    GetCharacterMovement()->StopMovementImmediately();
}

void AGoblin::Recovered_Implementation()
{
    OnRecovered.Broadcast();
}

bool AGoblin::CheckDead()
{
    Health <= 0 ? Dead = true : Dead = false;
    return Dead;
}

void AGoblin::EnterCombat(AActor* Enemy)
{
    UBlackboardComponent* Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);
    
    Blackboard->SetValueAsObject(EnemyKeyName, Enemy);

    AIState = EAIState::AIS_Combat;
    Blackboard->SetValueAsEnum(AIStateKeyName, (uint8) AIState);

    CombatState = EEnemyCombatState::ECS_Observing;
    Blackboard->SetValueAsEnum(CombatStateKeyName, (uint8) CombatState);
}

void AGoblin::ExitCombat()
{
    UBlackboardComponent* Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);

    Blackboard->ClearValue(EnemyKeyName);

    AIState = EAIState::AIS_Patrol;
    Blackboard->SetValueAsEnum(AIStateKeyName, (uint8) AIState);
}

void AGoblin::ShowHitVisuals_Implementation() {}

void AGoblin::Attack_Implementation()
{
    if (AttackComponent)
    {
        if (ActionState == EEnemyState::ES_Idle)
        {
            ActionState = EEnemyState::ES_Attacking;
            // ToDo: Don't hard-code attack names.
            AttackComponent->Attack(FString("Light01"));
        }
    }
}

void AGoblin::ResetAttack_Implementation()
{
    ActionState = EEnemyState::ES_Idle;
}

void AGoblin::AttackComplete_Implementation()
{

}

void AGoblin::ApplyAttackEffects_Implementation(const AActor* SourceActor, float Damage, const EHitEffect HitEffect, const FAttackEffect AttackEffect)
{
    DecrementHealth(Damage);

    if (SourceActor != nullptr)
    {
        // Find the direction in which we need to launch our character.
        FVector SourceLocation = SourceActor->GetActorLocation();
        FVector HitLocation = GetActorLocation();
        FVector LaunchDirection = HitLocation - SourceLocation;
        FRotator TurnRotation = FRotator(0.0f, (LaunchDirection * -1).Rotation().Yaw, 0.0f);
        LaunchDirection.Z = 0.0f;		// Do not want to launch the character up/down.
        LaunchDirection.Normalize();

        // Calculate launch velocity.
        FVector LaunchVelocity = LaunchDirection * AttackEffect.KnockbackForce;

        switch (HitEffect)
        {
        // Launch character backwards.
        case EHitEffect::HE_PushBack:
            LaunchCharacter(LaunchVelocity, true, false);
            SetActorRotation(TurnRotation);
            break;

        case EHitEffect::HE_KnockBack:
            break;

        case EHitEffect::HE_LaunchUp:
            break;

        default:
            break;
        }
    }
}

void AGoblin::KillActor_Implementation()
{
    Dead = true;
    DetachFromControllerPendingDestroy();
    DisableMovement();
}

void AGoblin::DecrementHealth_Implementation(float Damage)
{
    Health -= Damage;
}
