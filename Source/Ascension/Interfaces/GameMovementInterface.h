// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Globals.h"
#include "GameMovementInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameMovementInterface : public UInterface
{
    GENERATED_BODY()
};

/*
 * Interface to get the parameters related to custom movement of an entity.
 */
class ASCENSION_API IGameMovementInterface
{
    GENERATED_BODY()

public:
    /** Function to get the movement direction. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
    FVector GetMovementDirection() const;
};
