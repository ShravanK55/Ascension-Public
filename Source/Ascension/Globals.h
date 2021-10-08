#pragma once
#include "Ascension.h"
#include "Globals.generated.h"


UENUM(BlueprintType)
enum class EAIState : uint8
{
    AIS_Patrol			UMETA(DisplayName = "Patrol"),
    AIS_Combat			UMETA(DisplayName = "Combat")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
    ES_Idle				UMETA(DisplayName = "Idle"),
    ES_Dodging			UMETA(DisplayName = "Dodging"),
    ES_Attacking		UMETA(DisplayName = "Attacking")
};

UENUM(BlueprintType)
enum class EEnemyCombatState : uint8
{
    ECS_Observing		UMETA(DisplayName = "Observing"),
    ECS_Attacking		UMETA(DisplayName = "Attacking")
};

/*
 * Struct for custom movement parameters.
 */
USTRUCT(BlueprintType)
struct FCustomMovementParams
{
    GENERATED_USTRUCT_BODY()

    // Speed of movement during attack.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Speed = 1000.0f;

    // Max acceleration during this attack.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Acceleration = 20000.0f;

    // Turn rate during the attack.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float TurnRate = 2048.0f;

    // Maximum angle the character can rotate during the attack.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MaxTurnAngleDegrees = 0.0f;

};
