// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "PatrolComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASCENSION_API UPatrolComponent : public UActorComponent
{
    GENERATED_BODY()

public:	
    // Sets default values for this component's properties
    UPatrolComponent();

    /** Gets the patrol points. */
    UFUNCTION(BlueprintCallable, Category = "Helper")
    TArray<AActor*> GetPatrolPoints() const;

protected:
    UPROPERTY(EditInstanceOnly, Category = "Patrol Points")
    TArray<AActor*> PatrolPoints;
};
