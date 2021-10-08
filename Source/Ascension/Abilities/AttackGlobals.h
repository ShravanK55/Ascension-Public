// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/AbilityGlobals.h"
#include "Globals.h"
#include "AttackGlobals.generated.h"

struct FAttackMovementInfo;
struct FAttackEffectInfo;

/*
 * Class containing the globals for attack abilities.
 */
UCLASS()
class ASCENSION_API UAttackGlobals : public UAbilityGlobals
{
    GENERATED_BODY()
    
};

/*
 * Enum class depicting different hit effects.
 */
UENUM(BlueprintType)
enum class EHitEffect : uint8
{
    HE_PushBack			UMETA(DisplayName = "Push Back"),
    HE_LaunchUp			UMETA(DisplayName = "Launch Up"),
    HE_KnockBack		UMETA(DisplayName = "Knock Back")
};

/*
 * Enum class depicting different attack effects.
 */
USTRUCT(BlueprintType)
struct FAttackEffect
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
    float KnockbackForce;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
    float ZForce;
};

/*
 * Struct containing parameters involving movement during an attack.
 */
USTRUCT(BlueprintType)
struct FAttackMovementInfo
{
    GENERATED_USTRUCT_BODY()

    // Speed of movement during attack.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
    float Speed = 0.0f;

    // Max acceleration during this attack.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
    float Acceleration = 20000.0f;

    // Turn rate during the attack.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
    float TurnRate = 2048.0f;
};

/*
 * Struct containing parameters involving effects applied from an attack.
 * TODO: Rework this.
 */
USTRUCT(BlueprintType)
struct FAttackEffectInfo
{
    GENERATED_USTRUCT_BODY()

    // Type of effect on the enemy (Push back, knock back etc.)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
    EHitEffect HitEffect;

    // Damage dealt by the attack.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
    float Damage;

    // Effect of the attack on the enemy.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
    FAttackEffect AttackEffect;
};
