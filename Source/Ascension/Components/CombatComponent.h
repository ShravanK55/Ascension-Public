// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


/*
 * Component handling combat actions for an entity.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASCENSION_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
    /*
     * Called for the character to setup the specified attack.
     * @param AttackName    Name of the attack to setup.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
    void SetupAttack(const FString& AttackName);
    virtual void SetupAttack_Implementation(const FString& AttackName);

    /*
     * Called for the character to perform the specified attack.
     * @param AttackName    Name of the attack to perform.
     * @returns bool        Whether the attack was executed.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
    bool Attack(const FString& AttackName);
    virtual bool Attack_Implementation(const FString& AttackName);

    /*
     * Called perform a tick for the attack.
     * @param AttackName        Name of the attack to tick.
     * @param DeltaSeconds      Time elapsed since the last tick.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
    void TickAttack(const FString& AttackName, float DeltaSeconds);
    virtual void TickAttack_Implementation(const FString& AttackName, float DeltaSeconds);

    /*
     * Called for the character to finish the specified attack.
     * @param AttackName    Name of the attack to finish.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
    void FinishAttack(const FString& AttackName);
    virtual void FinishAttack_Implementation(const FString& AttackName);
	
    /*
     * Called for the character to setup the dodge.
     * @param DodgeName     Name of the dodge to setup.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dodge")
    void SetupDodge(const FString& DodgeName);
    virtual void SetupDodge_Implementation(const FString& DodgeName);

    /*
     * Called for the character to perform a dodge.
     * Dodge name is required in case there are different types of dodges.
     * @param DodgeName     Name of the dodge to perform. Defaults to "Dodge".
     * @returns bool        Whether the dodge was executed.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dodge")
    bool Dodge(const FString& DodgeName);
    virtual bool Dodge_Implementation(const FString& DodgeName);

    /*
     * Called perform a tick for the dodge.
     * @param AttackName        Name of the dodge to tick.
     * @param DeltaSeconds      Time elapsed since the last tick.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dodge")
    void TickDodge(const FString& DodgeName, float DeltaSeconds);
    virtual void TickDodge_Implementation(const FString& DodgeName, float DeltaSeconds);

    /*
     * Called for the character to finish the dodge.
     * @param DodgeName     Name of the dodge to finish.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dodge")
    void FinishDodge(const FString& DodgeName);
    virtual void FinishDodge_Implementation(const FString& DodgeName);

protected:
    /** The component's owner. */
    UPROPERTY(VisibleAnywhere, Category = "Owner")
    ACharacter* Owner;

};
