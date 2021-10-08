// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerGlobals.generated.h"

/*
 * Class containing globals for the player.
 */
UCLASS()
class ASCENSION_API UPlayerGlobals : public UObject
{
    GENERATED_BODY()
    
};

/*
 * Enum representing different input types.
 */
UENUM(BlueprintType)
enum class EInputType : uint8
{
    EIT_LightAttack         UMETA(DisplayName = "Light Attack"),
    EIT_SpecialAttack       UMETA(DisplayName = "Special Attack"), // TODO: Combine this with the light attack function.
    EIT_Stinger             UMETA(DisplayName = "Stinger"), // TODO: Combine this with the light attack function.
    EIT_Dodge               UMETA(DisplayName = "Dodge"),
    EIT_Move                UMETA(DisplayName = "Move"),
    EIT_Null                UMETA(DisplayName = "Null")
};
