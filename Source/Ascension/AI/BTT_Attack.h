// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Attack.generated.h"

/**
 * Task that makes an enemy perform an attack.
 */
UCLASS()
class ASCENSION_API UBTT_Attack : public UBTTaskNode
{
    GENERATED_BODY()
    
public:
    /** Key selector for signalling if the enemy wants to observe. */
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector WantsObserveKey;
    
protected:
    /** Starts execution for the task. */
    EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Progress")
    void AttackComplete(const bool Successful);

public:
    UBTT_Attack();

private:
    UBlackboardComponent* BlackboardComponent;
};
