// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "BTT_Attack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/AttackComponent.h"
#include "Entities/Enemies/Enemy.h"


UBTT_Attack::UBTT_Attack()
{
    bNotifyTick = true;
    bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    
    BlackboardComponent = OwnerComp.GetBlackboardComponent();
    ACharacter* ControlledPawn = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
    AEnemy* Enemy = Cast<AEnemy>(ControlledPawn);

    Enemy->Attack();
    UAttackComponent* AttackComponent = Enemy->FindComponentByClass<UAttackComponent>();

    if (AttackComponent)
    {
        AttackComponent->OnAttackComplete.AddDynamic(this, &UBTT_Attack::AttackComplete);
    }

    return EBTNodeResult::InProgress;
}

void UBTT_Attack::AttackComplete_Implementation(const bool Successful)
{
    BlackboardComponent->SetValueAsBool(WantsObserveKey.SelectedKeyName, true);

    UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
    if (OwnerComp)
    {
        ACharacter* ControlledPawn = Cast<ACharacter>(OwnerComp->GetAIOwner()->GetPawn());
        AEnemy* Enemy = Cast<AEnemy>(ControlledPawn);
        UAttackComponent* AttackComponent = Enemy->FindComponentByClass<UAttackComponent>();

        if (AttackComponent)
        {
            AttackComponent->OnAttackComplete.RemoveDynamic(this, &UBTT_Attack::AttackComplete);
        }

        FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
    }
}
