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


    // float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None) ;

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
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Ember Movement")
    bool Keyboard_Hook_W;

    /** Hook for A*/
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Ember Movement")
    bool Keyboard_Hook_A;

    /** Hook for S*/
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Ember Movement")
    bool Keyboard_Hook_S;

    /** Hook for D*/
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Ember Movement")
    bool Keyboard_Hook_D;

    /** LeftRight Resultant. -1 = left, +1 = right, 0 is center*/
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Ember Movement")
    float LR_Result;

    /** ForwardBack Resultant. -1 = back, +1 = forward, 0 is center*/
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Ember Movement")
    float FB_Result;

    /** Current Stance. 0 = light, 1 = balance, 2 = heavy*/
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Ember Sword")
    int32 CurrentStance;

    /** Activates SwordCalculations(). */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Ember Sword")
    bool bPlayerIsAttacking;

    /** Changes stance. Index based. 0 = light, 1 = balance, 2 = heavy */
    UFUNCTION(BlueprintCallable, Category = "Ember Sword")
    void ChangeStance(int32 Index);

    /** Attaches Sword and switches to default stance balance (1) */
    UFUNCTION(BlueprintCallable, Category = "Ember Sword")
    void AttachSword();

    /** Calculates Sword for 1 Tick (runs per tick by default) */
    UFUNCTION(BlueprintCallable, Category = "Ember Sword")
    void SwordCalculations(float DeltaTime);

    /** Calculates Movement for 1 Tick (runs per tick by default) */
    UFUNCTION(BlueprintCallable, Category = "Ember Character")
    void MovementCalculations(float DeltaTime);

    /** Hook for LeftClick */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Ember Character")
    bool bLeftClickPressed;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Animation)
    UAnimMontage* AttackMontage;

    UFUNCTION(BlueprintCallable, Category = "Ember Combat")
    void forwardAttack();

    UFUNCTION(BlueprintCallable, Category = "Ember Combat")
    void ResetMontageToIdle();

    UFUNCTION(BlueprintCallable, Category = "Ember Combat")
    void backAttack();

    UFUNCTION(BlueprintCallable, Category = "Ember Combat")
    void leftAttack();

    UFUNCTION(BlueprintCallable, Category = "Ember Combat")
    void rightAttack();

    UFUNCTION(BlueprintCallable, Category = "Ember Combat")
    void RationalizeAttack(int32 AttackID);
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
	
	
    void GetLeftRightResultant();
    void GetForwardBackResultant();


	void SwordDebug();

    void SetupVariables();

    // float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate);
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

