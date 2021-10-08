// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MovementIntent.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMovementIntent : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/** Interface to get the movement intent of an entity. */
class ASCENSION_API IMovementIntent
{
	GENERATED_IINTERFACE_BODY()

public:
	/** Function to get the movement intent. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
	FVector GetMovementIntent();
};
