// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "SwordModel.h"
#include "EmberUtilities.h"
//#include "EmberSwordCalculations.h";
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

    void AttachSword();
	void SwordDebug();

protected:
    // APawn interface
    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) OVERRIDE;
    // End of APawn interface
private:
    UClass* SwordBlueprint_GeneratedClass;
    ASwordModel * SwordModel;
    //EmberSwordCalculations * ESwordCalculations;
    bool DrawSwordDebug;
    bool DrawNewSwordDebug;
    FVector oldStart;
    FVector oldEnd;
    TArray <FVector> oldPoints;
    int Keyboard_WASD_Hook[4];
};

