// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Entities/GameCharacter.h"

#include "Globals.h"
#include "Interfaces/Damageable.h"
#include "Interfaces/GameMovementInterface.h"

#include "AscensionCharacter.generated.h"


/*
 * AscensionCharacter
 * This is the character that the player controls and plays the game with.
 */
UCLASS(config=Game)
class AAscensionCharacter : public AGameCharacter, public IDamageable, public IGameMovementInterface
{
    GENERATED_BODY()

    /** Camera boom positioning the camera behind the character */
    UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    /** Follow camera */
    UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

    /** Component handling character states. */
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UPlayerStateComponent* StateComponent;

    /** Component handling combat. */
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UPlayerCombatComponent* CombatComponent;

    /** Component handling abilities. */
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UPlayerAbilitySystemComponent* AbilitySystemComponent;

public:
    /** Name of the state component. */
    static FName StateComponentName;

    /** Name of the combat component. */
    static FName CombatComponentName;

    /** Name of the ability system component. */
    static FName AbilitySystemComponentName;

public:
    AAscensionCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** Character's BeginPlay function. */
    virtual void BeginPlay();

    /** Character's Tick function. */
    virtual void Tick(float DeltaSeconds);

public:
    /** Current health of the character.*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Parameters")
    float Health;

    /** Maximum health of the character.*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Parameters")
    float MaxHealth;

    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    float BaseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    float BaseLookUpRate;

    /** Speed at which the character walks. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float WalkSpeed;

    /** Normal speed of the character. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float NormalSpeed;

    /** Speed at which the character runs. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintSpeed;

    /** Acceleration of the character. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float NormalAcceleration;

    /** Base turn rate of the character. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float NormalTurnRate;

    /** Used to indicate to the animation blueprint whether the character should switch weapons. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool ShouldCharSwitch;

    /** The vector which holds player's forward movement intent. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FVector ForwardIntent;

    /** The vector which holds player's side movement intent. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FVector SideIntent;

    /** The vector holding the player's movement intent. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FVector MovementIntent;

    /** The direction the player should perform an action. */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
    FVector ActionDirection;

    /** Variable to detect if player is locked on to an enemy. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    bool LockedOn;

    /** The enemy that the player is locked on to. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    AActor* LockedActor;

protected:
    /** Resets HMD orientation in VR. */
    void OnResetVR();

    /** Called for forwards/backward input */
    void MoveForward(float Value);

    /** Called for side to side input */
    void MoveRight(float Value);

    /*
     * Called via input to turn at a given rate. 
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void TurnAtRate(float Rate);

    /*
     * Called via input to turn look up/down at a given rate. 
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void LookUpAtRate(float Rate);

    /** Called for the player to sprint. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void Sprint();

    /** Called for the player to stop sprinting. */
    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void StopSprinting();

    /** Called for the player to jump. */
    void Jump();

    /** Called for the player to stop jumping. */
    void StopJumping();

    /** Called for the player to perform a light attack. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void LightAttackPressed();

    /** Called for the player to perform a special attack. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void SpecialAttackPressed();

    /** Called for the player to dodge. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void DodgePressed();

    /** Called for the player to perform a light attack. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void LightAttackReleased();

    /** Called for the player to perform a special attack. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void SpecialAttackReleased();

    /** Called for the player to dodge. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void DodgeReleased();

    /** Called for the character to sheath/unsheath weapon. */
    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void SwitchWeapon();

    /** Called to stop player movement. */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StopMovement();

    /** Called to limit player movement to a certain speed. */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetMovementSpeed(float Speed);

    /** Called to reset player movement to normal speed. */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ResetMovementSpeed();

    /** Called to set acceleration to a value. */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetAcceleration(float Acceleration);

    /** Called to reset acceleration. */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ResetAcceleration();

    /** Called to stop player turning. */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StopTurning();

    /** Called to limit player turning to a certain rate. */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetTurningRate(float Rate);

    /** Called to reset player turning to normal rate. */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ResetTurningRate();

    /** Called to set gravity to a value. */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetGravity(float GravityValue);

    /** Called to reset gravity. */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ResetGravity();

    /** Handler for when a touch input begins. */
    void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

    /** Handler for when a touch input stops. */
    void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
    /*
     * Allows a Pawn to set up custom input bindings.
     * Called upon possession by a PlayerController, using the InputComponent created by CreatePlayerInputComponent().
     */
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    /** Function that returns the percentage of health the character has left. */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
    float GetHealthPercentage() const;

    /** Function that checks and sets whether the character is dead. */
    UFUNCTION(BlueprintCallable, Category = "Checking")
    bool CheckIsDead();

public:
    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    /** Returns FollowCamera subobject **/
    FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
    /*
     * Event called when the character completes switching.
     * Performs necessary actions after a switch is completed.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void SwitchComplete();

    /** Event called when character is hit by something. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void Impact(const FVector& Direction);

    /** Event called when recovering from an impact. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void Recovered();

public:
    // The following are events whose functionality is implemented in blueprint.

    /** Event called when the sword is sheathed. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void Sheathed();

    /** Event called when the sword is unsheathed. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void Unsheathed();

    /** Event called to play footstep sounds. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void FootstepSound();

    /** Event called to play a sword slash sound. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void SlashSound();

public:
    /* DAMAGEABLE INTERFACE FUNCTIONS */

    /** Returns the percentage of health remaining. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
    void GetHealthPercent(float& HealthPercent);
    virtual void GetHealthPercent_Implementation(float& HealthPercent) override;

    /** Applies the effects of an attack to the entity. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
    void ApplyHitEffect(const AActor* SourceActor, const float Damage, const EHitEffect HitEffect, const FAttackEffect AttackEffect);
    virtual void ApplyHitEffect_Implementation(const AActor* SourceActor, const float Damage, const EHitEffect HitEffect, const FAttackEffect AttackEffect) override;

    /** Shows the entity's health bar. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
    void ShowHealthBar();
    virtual void ShowHealthBar_Implementation() override;

    /** Hides the entity's health bar. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
    void HideHealthBar();
    virtual void HideHealthBar_Implementation() override;

    /** To check whether the entity is alive or not. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
    bool IsDead();
    virtual bool IsDead_Implementation() override;

    /** Function to get the movement direction. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface Functions")
    FVector GetMovementDirection() const;
    virtual FVector GetMovementDirection_Implementation() const;
};

