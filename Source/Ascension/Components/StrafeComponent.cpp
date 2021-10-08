// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "StrafeComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UStrafeComponent::UStrafeComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
}

void UStrafeComponent::BeginPlay()
{
    Owner = Cast<ACharacter>(GetOwner());
    Controller = Owner->GetController();
    AIController = Cast<AAIController>(Controller);
}

void UStrafeComponent::StrafeStart(AActor* Enemy)
{
    if (AIController)
    {
        AIController->SetFocus(Enemy);
    }
    else
    {
        Owner->GetCharacterMovement()->bUseControllerDesiredRotation = true;
    }
}

void UStrafeComponent::Strafe(AActor* Enemy)
{
    if (!AIController)
    {
        // Finding the direction to look at while strafing.
        FRotator Direction = UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), Enemy->GetActorLocation());

        // Turn toward the enemy.
        Controller->SetControlRotation(Direction);
    }
    
    // Move around enemy.
    Owner->AddMovementInput(Owner->GetActorRightVector());
}

void UStrafeComponent::StrafeEnd()
{
    if (AIController)
    {
        AIController->ClearFocus(EAIFocusPriority::Gameplay);
    }
    else
    {
        Owner->GetCharacterMovement()->bUseControllerDesiredRotation = false;
    }
}
