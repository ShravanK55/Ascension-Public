// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AscensionPlayerController.generated.h"

/*
 * Controller for the player character.
 */
UCLASS()
class ASCENSION_API AAscensionPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    /*
     * Constructor for the player controller.
     */
    AAscensionPlayerController();

public:
    /** Name of the input component. */
    static FName PlayerInputComponentName;

    /** Player input component. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
    class UPlayerInputComponent* PlayerInputComponent;

};
