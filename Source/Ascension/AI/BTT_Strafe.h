// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Strafe.generated.h"

/**
  * Task that makes a character strafe around a point/enemy.
  */
UCLASS()
class ASCENSION_API UBTT_Strafe : public UBTTaskNode
{
    GENERATED_BODY()

protected:
    /** Key selector containing the enemy to strafe around. */
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector EnemyKey;

    /** Key selector for signalling if the enemy wants to attack. */
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector WantsAttackKey;

    /** Total time to strafe for. */
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    float StrafeDuration;

    /** Time elapsed during strafing. */
    UPROPERTY(VisibleAnywhere, Category = "Blackboard")
    float StrafeElapsedTime = 0.0f;

protected:
    /** Starts execution for the task. */
    EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    /** Tick function. Updates every frame. */
    void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
    UBTT_Strafe();
};
