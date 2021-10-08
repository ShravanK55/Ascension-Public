// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "PlayerInputComponent.h"

#include "Components/Player/PlayerCombatComponent.h"
#include "Entities/Player/AscensionPlayerController.h"


UPlayerInputComponent::UPlayerInputComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    BufferSize = 20;
    InputValidity = 0.5f;
    InputTypeActiveMap = {
        {EInputType::EIT_LightAttack, false},
        {EInputType::EIT_SpecialAttack, false},
        {EInputType::EIT_Stinger, false},
        {EInputType::EIT_Dodge, false},
        {EInputType::EIT_Move, false}
    };
    InputBuffer.Reset(BufferSize);
}

void UPlayerInputComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    float CurrentTime = GetWorld()->GetTimeSeconds();

    // Process the normal input buffer.
    for (int Index = 0; Index < InputBuffer.Num(); Index++)
    {
        // Marking any inputs that aren't being updated anymore as inactive.
        bool IsInputActive = false;
        if (InputTypeActiveMap.Contains(InputBuffer[Index].InputType))
        {
            IsInputActive = InputTypeActiveMap[InputBuffer[Index].InputType];
        }
        UpdateLastInputAction(InputBuffer[Index].InputType, IsInputActive, CurrentTime);

        // Remove any input from the buffer if it isn't valid anymore.
        if ((CurrentTime - InputBuffer[Index].StartTime) > InputValidity)
        {
            if (!(InputBuffer[Index].Active))
            {
                EInputType InputType = InputBuffer[Index].InputType;
                UE_LOG(LogInputBuffer, Warning, TEXT("Removing input %s from the buffer."), *(INPUT_TYPE_STRING_MAP[InputType]))
                InputBuffer.RemoveAt(Index);
                Index--;
            }
        }
    }

    TryBufferedAction();
}


void UPlayerInputComponent::AddActionEvent(FActionEvent& ActionEvent)
{
    ActionEvents.Add(ActionEvent);
}

void UPlayerInputComponent::AddToBuffer(FInputAction& InputAction)
{
    if ((BufferSize != 0) && (InputBuffer.Num() == BufferSize))
    {
        // Only remove the earliest inactive input.
        int IndexToRemove = 0;

        for (int Index = 0; Index < InputBuffer.Num(); Index++)
        {
            if (!(InputBuffer[Index].Active))
            {
                IndexToRemove = Index;
                break;
            }
        }

        InputBuffer.RemoveAt(IndexToRemove);
    }
    
    InputBuffer.Add(InputAction);
}

void UPlayerInputComponent::ClearBuffer()
{
    UE_LOG(LogInputBuffer, Warning, TEXT("Clearing input buffer."))
    InputBuffer.Reset(BufferSize);
}

FInputAction UPlayerInputComponent::GetLastInputAction(const EInputType& InputType = EInputType::EIT_Null)
{
    // If the null type is passed, return the last input action.
    int LastIndex = GetLastInputActionIndex(InputType);
    if (LastIndex >= 0)
    {
        return InputBuffer[LastIndex];
    }

    return FInputAction();
}

int UPlayerInputComponent::GetLastInputActionIndex(const EInputType& InputType = EInputType::EIT_Null)
{
    // If the null type is passed, return the last input action.
    if (InputType == EInputType::EIT_Null)
    {
        int LastIndex = InputBuffer.Num() - 1;
        return LastIndex;
    }

    // Get the last input action based on the types.
    for (int Index = InputBuffer.Num() - 1; Index >= 0; Index--)
    {
        if (InputBuffer[Index].InputType == InputType)
        {
            return Index;
        }
    }

    return -1;
}

bool UPlayerInputComponent::UpdateLastInputAction(const EInputType& InputType, const bool& Active, const float& EndTime)
{
    // If an empty string is passed, return the last input action.
    int LastIndex = GetLastInputActionIndex(InputType);

    if (LastIndex >= 0)
    {
        FInputAction InputAction = InputBuffer[LastIndex];

        if (InputAction.Active)
        {
            InputAction.Active = Active;
            InputAction.EndTime = EndTime;
            InputBuffer[LastIndex] = InputAction;

            return true;
        }
    }

    return false;
}

void UPlayerInputComponent::BufferInput(const EInputType& InputType, bool Active)
{
    float CurrentTime = GetWorld()->GetTimeSeconds();

    // Input only needs to be buffered if its a new active action.
    if (!UpdateLastInputAction(InputType, Active, CurrentTime) && Active)
    {
        FInputAction InputAction = FInputAction(InputType, Active, CurrentTime, CurrentTime);
        UE_LOG(LogInputBuffer, Warning, TEXT("Adding input: %s"), *(INPUT_TYPE_STRING_MAP[InputAction.InputType]))
        AddToBuffer(InputAction);
        PrintBuffer();
    }

    InputTypeActiveMap[InputType] = Active;
}

TArray<FInputActionSequence> UPlayerInputComponent::GetValidInputSequences(const FActionEvent& ActionEvent) const
{
    TArray<FInputActionSequence> ValidInputSequences = TArray<FInputActionSequence>();
    TMap<EInputType, TArray<int>> InputTypeIndexMap = TMap<EInputType, TArray<int>>();

    TArray<EInputType> SequenceToMatch = ActionEvent.InputSequence;
    if (SequenceToMatch.Num() > 0)
    {
        // Store a mapping of each input action in the input buffer with the indices of that input action.
        for (int Index = 0; Index < InputBuffer.Num(); Index++)
        {
            EInputType InputType = InputBuffer[Index].InputType;

            if (!InputTypeIndexMap.Contains(InputType))
            {
                TArray<int> Indices = TArray<int>();
                Indices.Add(Index);
                InputTypeIndexMap.Add(InputType, Indices);
            }
            else
            {
                InputTypeIndexMap[InputType].Add(Index);
            }
        }

        if (InputTypeIndexMap.Num() > 0)
        {
            EInputType FirstInput = SequenceToMatch[0];
            TArray<FInputActionSequence> MatchedInputSequences = TArray<FInputActionSequence>();

            if (InputTypeIndexMap.Contains(FirstInput))
            {
                TArray<int> FirstInputIndices = InputTypeIndexMap[FirstInput];
                TArray<TArray<int>> InputSequencesToExplore = TArray<TArray<int>>();

                // Each index of the first action is a sequence to explore.
                for (int Index : FirstInputIndices)
                {
                    TArray<int> SequenceToExplore = TArray<int>();
                    SequenceToExplore.Add(Index);
                    InputSequencesToExplore.Add(SequenceToExplore);
                }

                // Find all input sequences in the buffer that match the required input sequence.
                // TODO: Rename variables for readability. Also possibly optimize this. Use bit operations?
                for (int Index = 0; Index < InputSequencesToExplore.Num(); Index++)
                {
                    TArray<int> CurrentInputSequence = InputSequencesToExplore[Index];

                    if (CurrentInputSequence.Num() < SequenceToMatch.Num())
                    {
                        // Get the indices of the next input action to match in the sequence.
                        EInputType NextInput = SequenceToMatch[CurrentInputSequence.Num() + 1];

                        if (InputTypeIndexMap.Contains(NextInput))
                        {
                            TArray<int> NextInputIndices = InputTypeIndexMap[NextInput];

                            // Check whether the indices of the next input action is valid (in proper sequence).
                            // TODO: Rename iterator variable for readability.
                            for (int NextIndex = 0; NextIndex < NextInputIndices.Num(); NextIndex++)
                            {
                                int LastCurrentIndex = CurrentInputSequence.Num() - 1;
                                if (NextInputIndices[NextIndex] > LastCurrentIndex)
                                {
                                    TArray<int> NextSequenceToExplore = CurrentInputSequence;
                                    NextSequenceToExplore.Add(NextInputIndices[NextIndex]);

                                    // If the number of elements in the sequence is equal, then we have a match.
                                    if (NextSequenceToExplore.Num() == SequenceToMatch.Num())
                                    {
                                        // Add the sequence to the array of matched sequences.
                                        TArray<FInputAction> MatchedActions = TArray<FInputAction>();

                                        for (int SequenceIndex = 0; SequenceIndex < NextSequenceToExplore.Num(); SequenceIndex++)
                                        {
                                            MatchedActions.Add(InputBuffer[NextSequenceToExplore[SequenceIndex]]);
                                        }

                                        FInputActionSequence MatchedSequence = FInputActionSequence(MatchedActions);
                                        MatchedInputSequences.Add(MatchedSequence);
                                    }
                                    else
                                    {
                                        InputSequencesToExplore.Add(NextSequenceToExplore);
                                    }
                                }
                            }
                        }
                        
                        // One of the input actions was not present in the buffer. In this case there is no need to explore further.
                        else
                        {
                            break;
                        }
                    }

                    // If the number of elements in the sequence is equal, then we have a match.
                    else
                    {
                        // Add the sequence to the array of matched sequences.
                        TArray<FInputAction> MatchedActions = TArray<FInputAction>();

                        for (int SequenceIndex = 0; SequenceIndex < CurrentInputSequence.Num(); SequenceIndex++)
                        {
                            MatchedActions.Add(InputBuffer[CurrentInputSequence[SequenceIndex]]);
                        }

                        FInputActionSequence MatchedSequence = FInputActionSequence(MatchedActions);
                        MatchedInputSequences.Add(MatchedSequence);
                    }
                }

                // Check whether matched sequences are valid.
                for (int Index = 0; Index < MatchedInputSequences.Num(); Index++)
                {
                    if (ActionEvent.CheckSequenceValidity(MatchedInputSequences[Index]))
                    {
                        ValidInputSequences.Add(MatchedInputSequences[Index]);
                    }
                }
            }
        }
    }

    // Sort valid input sequences based on their end times.
    // TODO: Implement something better than selection sort.
    for (int OuterIndex = 0; OuterIndex < ValidInputSequences.Num() - 1; OuterIndex++)
    {
        for (int InnerIndex = OuterIndex + 1; InnerIndex < ValidInputSequences.Num(); InnerIndex++)
        {
            if (ValidInputSequences[InnerIndex].GetEndTime() < ValidInputSequences[OuterIndex].GetStartTime())
            {
                FInputActionSequence TempSequence = ValidInputSequences[OuterIndex];
                ValidInputSequences[OuterIndex] = ValidInputSequences[InnerIndex];
                ValidInputSequences[InnerIndex] = TempSequence;
            }
        }
    }

    return ValidInputSequences;
}

bool UPlayerInputComponent::TryBufferedAction()
{
    FActionEvent ActionEventToExecute = FActionEvent();

    // Kept as a pointer mainly for comparison/initialization in the loop.
    FInputActionSequence EarliestExecutedSequence = FInputActionSequence();

    for (FActionEvent ActionEvent : ActionEvents)
    {
        TArray<FInputActionSequence> ValidSequences = GetValidInputSequences(ActionEvent);

        if (ValidSequences.Num() > 0)
        {
            if (EarliestExecutedSequence.InputActionSequence.Num() != 0)
            {
                if (ValidSequences[0].GetEndTime() < EarliestExecutedSequence.GetEndTime())
                {
                    ActionEventToExecute = ActionEvent;
                    EarliestExecutedSequence = ValidSequences[0];
                }
            }
            else
            {
                // Since the valid sequences are sorted by their end times already, the first sequence will be the
                // one that was executed the earliest.
                ActionEventToExecute = ActionEvent;
                EarliestExecutedSequence = ValidSequences[0];
            }
        }
    }

    AAscensionPlayerController* Controller = Cast<AAscensionPlayerController>(GetOwner());
    if ((ActionEventToExecute.PlayerAction != EPlayerInputAction::EPIA_Null) && (Controller))
    {
        APawn* Owner = Controller->GetPawn();

        if (Owner)
        {
            UPlayerCombatComponent* CombatComponent = Owner->FindComponentByClass<UPlayerCombatComponent>();

            if (CombatComponent)
            {
                bool Success = CombatComponent->PerformAction(ActionEventToExecute.PlayerAction);

                if (Success)
                {
                    UE_LOG(LogInputBuffer, Warning, TEXT("Action executed successfully."))
                    ClearBuffer();
                    PrintBuffer();
                    return true;
                }
            }
        }
    }

    return false;
}

void UPlayerInputComponent::PrintBuffer()
{
    FString InputBufferContents = FString("Input buffer contents: ");

    for (FInputAction InputAction : InputBuffer)
    {
        EInputType InputType = InputAction.InputType;
        InputBufferContents = InputBufferContents.Append(INPUT_TYPE_STRING_MAP[InputType]);
        InputBufferContents = InputBufferContents.Append(FString(" | "));
    }

    UE_LOG(LogInputBuffer, Warning, TEXT("%s"), *InputBufferContents)
}
