// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Ascension.h"
#include "AscensionCharacter.h"

#include "Globals.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "PlayerGlobals.h"
#include "Components/Player/PlayerAbilitySystemComponent.h"
#include "Components/Player/PlayerCombatComponent.h"
#include "Components/Player/PlayerInputComponent.h"
#include "Components/Player/PlayerStateComponent.h"
#include "Kismet/KismetMathLibrary.h"

//////////////////////////////////////////////////////////////////////////
// AAscensionCharacter

FName AAscensionCharacter::StateComponentName(TEXT("StateComponent"));
FName AAscensionCharacter::CombatComponentName(TEXT("CombatComponent"));
FName AAscensionCharacter::AbilitySystemComponentName(TEXT("AbilitySystemComponent"));

AAscensionCharacter::AAscensionCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Set size for collision capsule.
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Set our turn rates for input.
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;

    // Set character parameters.
    Health = 100.0f;
    MaxHealth = 100.0f;

    // Set movement speeds.
    WalkSpeed = 200.0f;
    NormalSpeed = 500.0f;
    SprintSpeed = 800.0f;
    NormalAcceleration = 2048.0f;
    NormalTurnRate = 540.0f;

    // Set gameplay variables.
    ShouldCharSwitch = false;

    // Camera lock-on variables.
    LockedOn = false;
    LockedActor = nullptr;

    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
    GetCharacterMovement()->JumpZVelocity = 600.f;
    GetCharacterMovement()->AirControl = 0.2f;

    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    // Create the player's state component.
    StateComponent = CreateDefaultSubobject<UPlayerStateComponent>(AAscensionCharacter::StateComponentName);

    // Create the player's combat component.
    CombatComponent = CreateDefaultSubobject<UPlayerCombatComponent>(AAscensionCharacter::CombatComponentName);

    // Create and initialize the player's ability system component.
    AbilitySystemComponent = CreateDefaultSubobject<UPlayerAbilitySystemComponent>(AAscensionCharacter::AbilitySystemComponentName);

    // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
    // are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAscensionCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // Set up gameplay key bindings
    check(PlayerInputComponent);

    // We have 2 versions of the rotation bindings to handle different kinds of devices differently
    // "turn" handles devices that provide an absolute delta, such as a mouse.
    // "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &AAscensionCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookUpRate", this, &AAscensionCharacter::LookUpAtRate);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AAscensionCharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &AAscensionCharacter::StopJumping);

    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AAscensionCharacter::Sprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AAscensionCharacter::StopSprinting);

    PlayerInputComponent->BindAction("LightAttack", IE_Pressed, this, &AAscensionCharacter::LightAttackPressed);
    PlayerInputComponent->BindAction("SpecialAttack", IE_Pressed, this, &AAscensionCharacter::SpecialAttackPressed);

    PlayerInputComponent->BindAction("LightAttack", IE_Released, this, &AAscensionCharacter::LightAttackReleased);
    PlayerInputComponent->BindAction("SpecialAttack", IE_Released, this, &AAscensionCharacter::SpecialAttackReleased);

    PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &AAscensionCharacter::DodgePressed);
    PlayerInputComponent->BindAction("Dodge", IE_Released, this, &AAscensionCharacter::DodgeReleased);

    PlayerInputComponent->BindAction("SheathWeapon", IE_Pressed, this, &AAscensionCharacter::SwitchWeapon);

    PlayerInputComponent->BindAxis("MoveForward", this, &AAscensionCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AAscensionCharacter::MoveRight);

    // handle touch devices
    PlayerInputComponent->BindTouch(IE_Pressed, this, &AAscensionCharacter::TouchStarted);
    PlayerInputComponent->BindTouch(IE_Released, this, &AAscensionCharacter::TouchStopped);

    // VR headset functionality
    PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AAscensionCharacter::OnResetVR);
}

void AAscensionCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AAscensionCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    MovementIntent = ForwardIntent + SideIntent;

    if (Controller != nullptr)
    {
        UPlayerInputComponent* PlayerInputComponent = Controller->FindComponentByClass<UPlayerInputComponent>();

        if (PlayerInputComponent)
        {
            if (!MovementIntent.IsNearlyZero(0.01f))
            {
                PlayerInputComponent->BufferInput(EInputType::EIT_Move, true);
            }
            else
            {
                PlayerInputComponent->BufferInput(EInputType::EIT_Move, false);
            }
        }
    }

    if (MovementIntent.IsNearlyZero(0.01f))
    {
        const FRotator YawRotation(0, GetActorRotation().Yaw, 0);
        MovementIntent = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    }
    else
    {
        MovementIntent.Normalize();
    }
}

void AAscensionCharacter::OnResetVR()
{
    UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AAscensionCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
    Jump();
}

void AAscensionCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
    StopJumping();
}


void AAscensionCharacter::MoveForward(float Value)
{
    // find out which way is forward
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    // get forward vector
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    ForwardIntent = Direction * Value;

    if (StateComponent)
    {
        if ((Controller != NULL) && (Value != 0.0f) &&
            StateComponent->GetCharacterState() == ECharacterState::CS_Idle)
        {
            AddMovementInput(Direction, Value);
        }
    }
}

void AAscensionCharacter::MoveRight(float Value)
{
    // find out which way is right
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    // get right vector 
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    SideIntent = Direction * Value;

    if (StateComponent)
    {
        if ((Controller != NULL) && (Value != 0.0f) &&
            StateComponent->GetCharacterState() == ECharacterState::CS_Idle)
        {
            // add movement in that direction
            AddMovementInput(Direction, Value);
        }
    }
}


void AAscensionCharacter::TurnAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAscensionCharacter::LookUpAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAscensionCharacter::Sprint_Implementation()
{
    if (StateComponent)
    {
        if (StateComponent->GetCharacterState() == ECharacterState::CS_Idle &&
            StateComponent->GetMovementMode() == EMovementMode::MOVE_Walking)
        {
            UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
            MovementComponent->MaxWalkSpeed = SprintSpeed;
        }
    }
}

void AAscensionCharacter::StopSprinting()
{
    if (StateComponent)
    {
        if (StateComponent->GetCharacterState() == ECharacterState::CS_Idle)
        {
            UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
            MovementComponent->MaxWalkSpeed = NormalSpeed;
        }
    }
}

void AAscensionCharacter::Jump()
{
    if (StateComponent)
    {
        if (StateComponent->GetCharacterState() == ECharacterState::CS_Idle)
        {
            ACharacter::Jump();
        }
    }
}

void AAscensionCharacter::StopJumping()
{
    if (StateComponent)
    {
        if (StateComponent->GetCharacterState() == ECharacterState::CS_Idle)
        {
            ACharacter::StopJumping();
        }
    }
}

void AAscensionCharacter::LightAttackPressed_Implementation()
{
    if (Controller != nullptr)
    {
        UPlayerInputComponent* PlayerInputComponent = Controller->FindComponentByClass<UPlayerInputComponent>();

        if (PlayerInputComponent)
        {
            PlayerInputComponent->BufferInput(EInputType::EIT_LightAttack, true);
        }
    }
}

void AAscensionCharacter::SpecialAttackPressed_Implementation()
{
    if (Controller != nullptr)
    {
        UPlayerInputComponent* PlayerInputComponent = Controller->FindComponentByClass<UPlayerInputComponent>();

        if (PlayerInputComponent)
        {
            PlayerInputComponent->BufferInput(EInputType::EIT_SpecialAttack, true);
        }
    }
}

void AAscensionCharacter::DodgePressed_Implementation()
{
    if (Controller != nullptr)
    {
        UPlayerInputComponent* PlayerInputComponent = Controller->FindComponentByClass<UPlayerInputComponent>();

        if (PlayerInputComponent)
        {
            PlayerInputComponent->BufferInput(EInputType::EIT_Dodge, true);
        }
    }
}

void AAscensionCharacter::LightAttackReleased_Implementation()
{
    if (Controller != nullptr)
    {
        UPlayerInputComponent* PlayerInputComponent = Controller->FindComponentByClass<UPlayerInputComponent>();

        if (PlayerInputComponent)
        {
            PlayerInputComponent->BufferInput(EInputType::EIT_LightAttack, false);
            PlayerInputComponent->TryBufferedAction();
        }
    }
}

void AAscensionCharacter::SpecialAttackReleased_Implementation()
{
    if (Controller != nullptr)
    {
        UPlayerInputComponent* PlayerInputComponent = Controller->FindComponentByClass<UPlayerInputComponent>();

        if (PlayerInputComponent)
        {
            PlayerInputComponent->BufferInput(EInputType::EIT_SpecialAttack, false);
            PlayerInputComponent->TryBufferedAction();
        }
    }
}

void AAscensionCharacter::DodgeReleased_Implementation()
{
    if (Controller != nullptr)
    {
        UPlayerInputComponent* PlayerInputComponent = Controller->FindComponentByClass<UPlayerInputComponent>();

        if (PlayerInputComponent)
        {
            PlayerInputComponent->BufferInput(EInputType::EIT_Dodge, false);
            PlayerInputComponent->TryBufferedAction();
        }
    }
}

void AAscensionCharacter::SwitchWeapon()
{
    if (StateComponent)
    {
        if (StateComponent->GetCharacterState() == ECharacterState::CS_Idle &&
            StateComponent->GetMovementMode() == EMovementMode::MOVE_Walking)
        {
            switch (StateComponent->GetWeaponState())
            {
            case EWeaponState::WS_Sheathed:
                StateComponent->SetWeaponState(EWeaponState::WS_Unsheathed);
                break;

            case EWeaponState::WS_Unsheathed:
                StateComponent->SetWeaponState(EWeaponState::WS_Sheathed);
                break;
            }

            StateComponent->SetCharacterState(ECharacterState::CS_Switching);
        }
    }

    StopMovement();
    ShouldCharSwitch = true;
    SetTurningRate(100.0f);
}

void AAscensionCharacter::StopMovement()
{
    GetCharacterMovement()->MaxWalkSpeed = 0.0f;
}

void AAscensionCharacter::SetMovementSpeed(float Speed)
{
    GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void AAscensionCharacter::ResetMovementSpeed()
{
    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

void AAscensionCharacter::SetAcceleration(float Acceleration)
{
    GetCharacterMovement()->MaxAcceleration = Acceleration;
}

void AAscensionCharacter::ResetAcceleration()
{
    GetCharacterMovement()->MaxAcceleration = NormalAcceleration;
}

void AAscensionCharacter::StopTurning()
{
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
}

void AAscensionCharacter::SetTurningRate(float Rate)
{
    GetCharacterMovement()->RotationRate = FRotator(0.0f, Rate, 0.0f);
}

void AAscensionCharacter::ResetTurningRate()
{
    GetCharacterMovement()->RotationRate = FRotator(0.0f, NormalTurnRate, 0.0f);
}

void AAscensionCharacter::SetGravity(float GravityValue)
{
    GetCharacterMovement()->GravityScale = GravityValue;
}

void AAscensionCharacter::ResetGravity()
{
    GetCharacterMovement()->GravityScale = 1.0f;
}

float AAscensionCharacter::GetHealthPercentage() const
{
    return Health / MaxHealth;
}

bool AAscensionCharacter::CheckIsDead()
{
    if (Health <= 0)
    {
        StateComponent->SetCharacterState(ECharacterState::CS_Dead);
        return true;
    }

    return false;
}

void AAscensionCharacter::SwitchComplete_Implementation()
{
    if (StateComponent)
    {
        StateComponent->SetCharacterState(ECharacterState::CS_Idle);
    }

    ShouldCharSwitch = false;
    ResetMovementSpeed();
    ResetTurningRate();
}

void AAscensionCharacter::Impact_Implementation(const FVector& Direction)
{
    if (StateComponent)
    {
        switch (StateComponent->GetCharacterState())
        {
        case ECharacterState::CS_Idle:
            break;
        case ECharacterState::CS_Attacking:
            break;
        case ECharacterState::CS_Dodging:
            break;
        case ECharacterState::CS_Switching:
            SwitchComplete();
            break;
        case ECharacterState::CS_Stunned:
            break;
        }

        StateComponent->SetCharacterState(ECharacterState::CS_Stunned);
    }
    
    LaunchCharacter(Direction, true, false);
}

void AAscensionCharacter::Recovered_Implementation()
{
    if (StateComponent)
    {
        StateComponent->SetCharacterState(ECharacterState::CS_Idle);
    }
}

void AAscensionCharacter::Sheathed_Implementation() {}
void AAscensionCharacter::Unsheathed_Implementation() {}
void AAscensionCharacter::FootstepSound_Implementation() {}
void AAscensionCharacter::SlashSound_Implementation() {}

void AAscensionCharacter::GetHealthPercent_Implementation(float& HealthPercent)
{
    HealthPercent = GetHealthPercentage();
}

void AAscensionCharacter::ApplyHitEffect_Implementation(const AActor* SourceActor, const float Damage, const EHitEffect HitEffect, const FAttackEffect AttackEffect)
{
    if (!(StateComponent->GetCharacterState() == ECharacterState::CS_Dead))
    {
        FVector AttackDirection = GetActorForwardVector() * -1;

        if (SourceActor != nullptr)
        {
            // Find the direction in which we need to launch our character.
            FVector SourceLocation = SourceActor->GetActorLocation();
            FVector HitLocation = GetActorLocation();
            FVector Direction = HitLocation - SourceLocation;
            Direction.Z = 0.0f;		// Do not want to launch the character up/down.
            Direction.Normalize();

            // Turn the character to the direction of the attack.
            SetActorRotation((Direction * -1).Rotation());

            // Calculate launch velocity.
            AttackDirection = Direction * AttackEffect.KnockbackForce;
        }
        
        switch (HitEffect)
        {
        case EHitEffect::HE_PushBack:
            Impact(AttackDirection);
            break;
        case EHitEffect::HE_KnockBack:
            break;
        case EHitEffect::HE_LaunchUp:
            break;
        }
        
        Health -= Damage;
    }
}

bool AAscensionCharacter::IsDead_Implementation()
{
    return (StateComponent->GetCharacterState() == ECharacterState::CS_Dead);
}

void AAscensionCharacter::ShowHealthBar_Implementation() {}
void AAscensionCharacter::HideHealthBar_Implementation() {}

FVector AAscensionCharacter::GetMovementDirection_Implementation() const
{
    if (LockedOn && LockedActor != nullptr)
    {
        FVector TargetDirection = LockedActor->GetActorLocation() - GetActorLocation();
        TargetDirection.Normalize();
        return TargetDirection;
    }

    return MovementIntent;
}
