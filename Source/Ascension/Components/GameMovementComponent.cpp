// Fill out your copyright notice in the Description page of Project Settings.

#include "Ascension.h"
#include "GameMovementComponent.h"

#include "Abilities/Ability.h"
#include "Components/GameAbilitySystemComponent.h"
#include "Interfaces/GameMovementInterface.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UGameMovementComponent::UGameMovementComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // Set movement speeds.
    DefaultSpeed = MaxWalkSpeed;
    DefaultAcceleration = MaxAcceleration;
    DefaultTurnRate = RotationRate.Yaw;
    DefaultOrientRotationToMovement = true;
    MovementDirection = FVector();
}

// Called when the game starts
void UGameMovementComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UGameMovementComponent::SetupControlledMovement(float TargetSpeed, float TargetAcceleration, float TargetTurnRate,
                                                     float MaxTurnAngleDegrees = 0.0f)
{
    SetMovementSpeed(TargetSpeed);
    SetAcceleration(TargetAcceleration);
    SetTurningRate(TargetTurnRate);

    AActor* Owner = GetOwner();
    if (Owner->Implements<UGameMovementInterface>())
    {
        FVector ActorForwardVector = Owner->GetActorForwardVector();
        FVector DesiredMovementDirection = IGameMovementInterface::Execute_GetMovementDirection(Owner);

        ActorForwardVector.Normalize();
        DesiredMovementDirection.Normalize();

        float Angle = UKismetMathLibrary::DegAcos(FVector::DotProduct(ActorForwardVector, DesiredMovementDirection));
        FVector CrossProductVector = FVector::CrossProduct(ActorForwardVector, DesiredMovementDirection);

        Angle = Angle < MaxTurnAngleDegrees ? Angle : MaxTurnAngleDegrees;
        Angle = CrossProductVector.Z > 0 ? Angle : -Angle;

        FVector DirectionToMove = ActorForwardVector.RotateAngleAxis(Angle, FVector(0.0f, 0.0f, 1.0f));
        DirectionToMove.Normalize();

        SetMovementDirection(DirectionToMove);
    }
    else
    {
        SetMovementDirection(Owner->GetActorForwardVector());
    }
}

void UGameMovementComponent::SetupControlledMovementAbility(FString AbilityName)
{
    AActor* Owner = GetOwner();
    UGameAbilitySystemComponent* AbilitySystemComponent = Owner->FindComponentByClass<UGameAbilitySystemComponent>();

    const UAbility* Ability = AbilitySystemComponent->GetActiveAbility(AbilityName);
    if (Ability != nullptr)
    {
        FCustomMovementParams MovementParams = Ability->GetMovementParams();
        SetupControlledMovement(MovementParams.Speed, MovementParams.Acceleration, MovementParams.TurnRate,
                                MovementParams.MaxTurnAngleDegrees);
    }
}

void UGameMovementComponent::ControlledMove(FVector MovementVector, float DeltaTime, bool SetVelocity = false)
{
    // Movement direction is the direction in relation to which the controlled movement is applied.
    FRotator MovementRotation = MovementDirection.Rotation();
    FVector ForwardVector = UKismetMathLibrary::GetForwardVector(MovementRotation) * MovementVector.X;
    FVector SideVector = UKismetMathLibrary::GetRightVector(MovementRotation) * MovementVector.Y;
    FVector UpVector = UKismetMathLibrary::GetUpVector(MovementRotation) * MovementVector.Z;

    // When there is only Z movement, we should not rotate the character along with the direction of movement.
    // This is due to an issue where the character always turns toward one direction when only Z movement is present.
    if (ForwardVector.IsNearlyZero() && SideVector.IsNearlyZero() && !UpVector.IsNearlyZero())
    {
        bOrientRotationToMovement = false;
    }
    else
    {
        bOrientRotationToMovement = DefaultOrientRotationToMovement;
    }

    FVector Direction = ForwardVector + SideVector + UpVector;

    if (SetVelocity)
    {
        Velocity = Direction * MaxWalkSpeed;
    }
    else
    {
        AddInputVector(Direction, false);
    }
}

void UGameMovementComponent::FinishControlledMovement()
{
    ResetMovementSpeed();
    ResetTurningRate();
    ResetAcceleration();
    ResetFlyable();
    bOrientRotationToMovement = DefaultOrientRotationToMovement;
}

void UGameMovementComponent::SetMovementSpeed(float Speed)
{
    MaxWalkSpeed = Speed;
    MaxFlySpeed = Speed;
}

void UGameMovementComponent::ResetMovementSpeed()
{
    MaxWalkSpeed = DefaultSpeed;
    MaxFlySpeed = DefaultSpeed;
}

void UGameMovementComponent::SetAcceleration(float TargetAcceleration)
{
    MaxAcceleration = TargetAcceleration;
}

void UGameMovementComponent::ResetAcceleration()
{
    MaxAcceleration = DefaultAcceleration;
}

void UGameMovementComponent::SetTurningRate(float Rate)
{
    RotationRate = FRotator(0.0f, Rate, 0.0f);
}

void UGameMovementComponent::ResetTurningRate()
{
    RotationRate = FRotator(0.0f, DefaultTurnRate, 0.0f);
}

void UGameMovementComponent::SetMovementDirection(FVector Direction)
{
    MovementDirection = Direction;
}

void UGameMovementComponent::SetGravity(float GravityValue)
{
    GravityScale = GravityValue;
}

void UGameMovementComponent::ResetGravity()
{
    GravityScale = 1.0f;
}

void UGameMovementComponent::SetFlyable()
{
    if (MovementMode != EMovementMode::MOVE_Flying)
    {
        MovementMode = EMovementMode::MOVE_Flying;
    }
}

void UGameMovementComponent::ResetFlyable()
{
    if (MovementMode == EMovementMode::MOVE_Flying)
    {
        MovementMode = EMovementMode::MOVE_Falling;
    }
}
