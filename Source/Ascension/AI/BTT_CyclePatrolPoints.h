// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTT_CyclePatrolPoints.generated.h"

/**
  * Task that sets the next point that a character has to patrol to.
  */
UCLASS()
class ASCENSION_API UBTT_CyclePatrolPoints : public UBTTaskNode
{
    GENERATED_BODY()

protected:
    /** Key selector containing the patrol point to travel to. */
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector PatrolPointKey;

    /** Key selector containing the index of the patrol point. */
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector NextPatrolPointIndexKey;

protected:
    /** Starts execution for the task. */
    EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
