// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "BTT_Strafe.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Components/StrafeComponent.h"


UBTT_Strafe::UBTT_Strafe()
{
    bNotifyTick = true;
}

EBTNodeResult::Type UBTT_Strafe::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    UStrafeComponent* StrafeComponent = OwnerComp.GetAIOwner()->GetPawn()->FindComponentByClass<UStrafeComponent>();
    AActor* Enemy = Cast<AActor>(BlackboardComponent->GetValueAsObject(EnemyKey.SelectedKeyName));

    if (StrafeComponent)
    {
        StrafeComponent->StrafeStart(Enemy);
    }

    return EBTNodeResult::InProgress;
}

void UBTT_Strafe::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    StrafeElapsedTime += DeltaSeconds;

    if (StrafeElapsedTime >= StrafeDuration)
    {
        StrafeElapsedTime = 0.0f;

        BlackboardComponent->SetValueAsBool(WantsAttackKey.SelectedKeyName, true);

        UStrafeComponent* StrafeComponent = OwnerComp.GetAIOwner()->GetPawn()->FindComponentByClass<UStrafeComponent>();
        if (StrafeComponent)
        {
            StrafeComponent->StrafeEnd();
        }

        OwnerComp.OnTaskFinished(this, EBTNodeResult::Succeeded);
    }
    else
    {
        AActor* Enemy = Cast<AActor>(BlackboardComponent->GetValueAsObject(EnemyKey.SelectedKeyName));
        UStrafeComponent* StrafeComponent = OwnerComp.GetAIOwner()->GetPawn()->FindComponentByClass<UStrafeComponent>();

        if (StrafeComponent)
        {
            StrafeComponent->Strafe(Enemy);
        }
    }
}
