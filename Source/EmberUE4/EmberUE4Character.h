// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "SwordModel.h"
#include "EmberUtilities.h"
#include "EmberUE4Character.generated.h"


UCLASS(config=Game)
class AEmberUE4Character : public ACharacter
{
    GENERATED_UCLASS_BODY()

    virtual void BeginPlay() OVERRIDE;
    virtual void Tick(float) OVERRIDE;

    /** Camera boom positioning the camera behind the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    TSubobjectPtr<class USpringArmComponent> CameraBoom;

    /** Follow camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    TSubobjectPtr<class UCameraComponent> FollowCamera;

    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    float BaseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    float BaseLookUpRate;

    /** Hook for W*/
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Movement Variables")
    bool Keyboard_Hook_W;

    /** Hook for A*/
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Movement Variables")
    bool Keyboard_Hook_A;

    /** Hook for S*/
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Movement Variables")
    bool Keyboard_Hook_S;

    /** Hook for D*/
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Movement Variables")
    bool Keyboard_Hook_D;

    /** Current Stance. 0 = light, 1 = balance, 2 = heavy*/
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Sword Variables")
    int32 CurrentStance;

    /** Activates SwordCalculations(). */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Sword Variables")
    bool bPlayerIsAttacking;

    /** Changes stance. Index based. 0 = light, 1 = balance, 2 = heavy */
    UFUNCTION(BlueprintCallable, Category = "Sword Functions")
    void ChangeStance(int32 Index);

    /** Attaches Sword and switches to default stance balance (1) */
    UFUNCTION(BlueprintCallable, Category = "Sword Functions")
    void AttachSword();

    /** Calculates Sword for 1 Tick (runs per tick by default) */
    UFUNCTION(BlueprintCallable, Category = "Sword Functions")
    void SwordCalculations(float DeltaTime);

    /** Hook for LeftClick */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Character Variables")
    bool bLeftClickPressed;


protected:

    /** Called for forwards/backward input */
    void MoveForward(float Value);

    /** Called for side to side input */
    void MoveRight(float Value);

    /**
     * Called via input to turn at a given rate.
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void TurnAtRate(float Rate);

    /**
     * Called via input to turn look up/down at a given rate.
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void LookUpAtRate(float Rate);

    /** Handler for when a touch input begins. */
    void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

    void LightStance();
    void MediumStance();
    void HeavyStance();

	void W_Pressed();
	void A_Pressed();
	void S_Pressed();
	void D_Pressed();

    void LeftClickPressed();
    void PrepareAttack();

	void SwordDebug();

    void SetupVariables();
protected:
    // APawn interface
    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) OVERRIDE;
    // End of APawn interface
private:
    UClass* SwordBlueprint_GeneratedClass;
    ASwordModel * SwordModel;
    //UEmberUtilitiesBP * EBP;
    bool DrawSwordDebug;
    bool DrawNewSwordDebug;
    FVector oldStart;
    FVector oldEnd;
    TArray <FVector> oldPoints;
    
};

