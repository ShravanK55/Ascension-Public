// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Enemy.h"
#include "GameFramework/Character.h"
#include "Interfaces/Damageable.h"
#include "Interfaces/Lockable.h"
#include "Runtime/AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Goblin.generated.h"


/** Goblin
  * A greatsword wielding enemy controlled by AI.
  */
UCLASS()
class ASCENSION_API AGoblin : public AEnemy, public IDamageable, public ILockable
{
    GENERATED_BODY()
    
    /** Component handling attacks. */
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UAttackComponent* AttackComponent;

    /** Component handling abilities. */
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UGameAbilitySystemComponent* AbilitySystemComponent;

public:
    /** Name of the attack component. */
    static FName AttackComponentName;

    /** Name of the ability system component. */
    static FName AbilitySystemComponentName;

public:
    AGoblin(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** Current health of the character.*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Parameters")
    float Health;

    /** Maximum health of the character.*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Parameters")
    float MaxHealth;

    /** General state of the AI.*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Parameters")
    EAIState AIState;

    /** Combat state of the AI. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Parameters")
    EEnemyCombatState CombatState;

    /** State of actions being performed by the AI. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Parameters")
    EEnemyState ActionState;

    /** Points which the entity patrols. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
    TArray<AActor*> PatrolPoints;

    /** Blackboard Key Names */

    /** AI State key name.*/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blackboard Keys")
    FName AIStateKeyName = FName("AIState");

    /** AI Combat State key name.*/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blackboard Keys")
    FName CombatStateKeyName = FName("AICombatState");

    /** Enemy key name.*/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blackboard Keys")
    FName EnemyKeyName = FName("Enemy");

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

public:
    /** Implementation of attack. */
    virtual void Attack_Implementation() override;

    /** Event called for character to reset attack. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void ResetAttack();

    /** Event called when an attack is complete. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void AttackComplete();

public:
    /** Event called to play footstep sounds. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void FootstepSound();

    /** Enables entity movement. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void EnableMovement();

    /** Disables entity movement. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void DisableMovement();

    /** Signals when character has recovered from a hit. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void Recovered();

protected:
    /** Checks and updates whether the goblin is dead. */
    UFUNCTION(BlueprintCallable, Category = "Helper")
    bool CheckDead();

    /** Makes changes to enter the combat state. */
    UFUNCTION(BlueprintCallable, Category = "Helper")
    void EnterCombat(AActor* Enemy);

    /** Makes changes to exit the combat state. */
    UFUNCTION(BlueprintCallable, Category = "Helper")
    void ExitCombat();

    /** Displays the visuals required when the character is attacked. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void ShowHitVisuals();

    /** Applies the effects of an attack onto the character. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void ApplyAttackEffects(const AActor* SourceActor, float Damage, const EHitEffect HitEffect, const FAttackEffect AttackEffect);

    /** Decrements the entity's health by the specified amount. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void DecrementHealth(float Damage);

    /** Performs the necessary steps to kill the actor. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
    void KillActor();

private:
    
};
