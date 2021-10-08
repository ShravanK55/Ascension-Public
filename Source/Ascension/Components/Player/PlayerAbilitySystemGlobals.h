// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerAbilitySystemGlobals.generated.h"


/*
 * Class containing globals for the player ability system.
 */
UCLASS()
class ASCENSION_API UPlayerAbilitySystemGlobals : public UObject
{
    GENERATED_BODY()
    
};


/*
 * Enum representing different abilities.
 */
UENUM(BlueprintType)
enum class EPlayerAbility : uint8
{
    EPA_Light01                 UMETA(DisplayName = "Light Attack 01"),
    EPA_Light02                 UMETA(DisplayName = "Light Attack 02"),
    EPA_Light03A                UMETA(DisplayName = "Light Attack 03 A"),
    EPA_Light03B                UMETA(DisplayName = "Light Attack 03 B"),
    EPA_Light04B                UMETA(DisplayName = "Light Attack 04 B"),
    EPA_StaticLaunch            UMETA(DisplayName = "Static Launch Attack"),
    EPA_AerialLaunch            UMETA(DisplayName = "Aerial Launch Attack"),
    EPA_AirSlam                 UMETA(DisplayName = "Air Slam Attack"),
    EPA_Stinger                 UMETA(DisplayName = "Stinger"),
    EPA_AirLight01              UMETA(DisplayName = "Air Light Attack 01"),
    EPA_AirLight02              UMETA(DisplayName = "Air Light Attack 02"),
    EPA_AirLight03              UMETA(DisplayName = "Air Light Attack 03"),
    EPA_AirLight04              UMETA(DisplayName = "Air Light Attack 04"),
    EPA_Dodge                   UMETA(DisplayName = "Dodge"),
    EPA_Null                    UMETA(DisplayName = "Null")
};


/*
 * Enum representing different types of input actions performed by the player.
 */
UENUM(BlueprintType)
enum class EPlayerInputAction : uint8
{
    EPIA_LightAttack                 UMETA(DisplayName = "Light Attack"),
    EPIA_StaticLaunch                UMETA(DisplayName = "Static Launch Attack"),
    EPIA_AerialLaunch                UMETA(DisplayName = "Aerial Launch Attack"),
    EPIA_Stinger                     UMETA(DisplayName = "Stinger"),
    EPIA_Dodge                       UMETA(DisplayName = "Dodge"),
    EPIA_Null                        UMETA(DisplayName = "Null")
};


/*
 * Map of player ability identifiers to the respective ability name.
 */
static const TMap<EPlayerAbility, FString> ABILITY_ID_NAME_MAP = {
    {EPlayerAbility::EPA_Light01, FString("Light01")},
    {EPlayerAbility::EPA_Light02, FString("Light02")},
    {EPlayerAbility::EPA_Light03A, FString("Light03A")},
    {EPlayerAbility::EPA_Light03B, FString("Light03B")},
    {EPlayerAbility::EPA_Light04B, FString("Light04B")},
    {EPlayerAbility::EPA_StaticLaunch, FString("StaticLaunch")},
    {EPlayerAbility::EPA_AerialLaunch, FString("AerialLaunch")},
    {EPlayerAbility::EPA_AirSlam, FString("AirSlam")},
    {EPlayerAbility::EPA_Stinger, FString("Stinger")},
    {EPlayerAbility::EPA_AirLight01, FString("AirLight01")},
    {EPlayerAbility::EPA_AirLight02, FString("AirLight02")},
    {EPlayerAbility::EPA_AirLight03, FString("AirLight03")},
    {EPlayerAbility::EPA_AirLight04, FString("AirLight04")},
    {EPlayerAbility::EPA_Dodge, FString("Dodge")},
    {EPlayerAbility::EPA_Null, FString("")}
};


/*
 * Enum representing the different chain states that a player can have.
 */
UENUM(BlueprintType)
enum class EChainState : uint8
{
    ECS_Default                     UMETA(DisplayName = "Default Chain State"),
    ECS_ComboB                      UMETA(DisplayName = "Combo B"),
    ECS_None                        UMETA(DisplayName = "None")
};
