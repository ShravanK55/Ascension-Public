// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "AscensionPlayerController.h"

#include "Components/Player/PlayerInputComponent.h"


FName AAscensionPlayerController::PlayerInputComponentName(TEXT("PlayerInputComponent"));

AAscensionPlayerController::AAscensionPlayerController()
{
    PlayerInputComponent = CreateDefaultSubobject<UPlayerInputComponent>(PlayerInputComponentName);
}
