// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "BTT_CyclePatrolPoints.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/PatrolComponent.h"
#include "AIController.h"


EBTNodeResult::Type UBTT_CyclePatrolPoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    AActor* Owner = OwnerComp.GetAIOwner()->GetPawn();

    UPatrolComponent* PatrolComponent = Owner->FindComponentByClass<UPatrolComponent>();

    if (PatrolComponent)
    {
        TArray<AActor*> Points = PatrolComponent->GetPatrolPoints();
        int Index = BlackboardComponent->GetValueAsInt(NextPatrolPointIndexKey.SelectedKeyName);

        if (Points.IsValidIndex(Index))
        {
            BlackboardComponent->SetValueAsObject(PatrolPointKey.SelectedKeyName, Points[Index]);
        }

        int NextIndex = (Index + 1) % Points.Num();
        BlackboardComponent->SetValueAsInt(NextPatrolPointIndexKey.SelectedKeyName, NextIndex);

        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
