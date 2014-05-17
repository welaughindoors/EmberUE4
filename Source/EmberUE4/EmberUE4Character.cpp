// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "EmberUE4.h"
#include "EmberUE4Character.h"

//////////////////////////////////////////////////////////////////////////
// AEmberUE4Character

AEmberUE4Character::AEmberUE4Character(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Set size for collision capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	CharacterMovement->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	CharacterMovement->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	CharacterMovement->JumpZVelocity = 600.f;
	CharacterMovement->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUseControllerViewRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUseControllerViewRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	
}

void AEmberUE4Character::AttachSword()
{
	UWorld* const World = GetWorld();
	if(!SwordModel)
	{
		SwordModel = World->SpawnActor<ASwordModel>(ASwordModel::StaticClass());
		SwordModel->AttachRootComponentTo(this->Mesh, FName(TEXT("RIGHT_HAND_ATTACH")));
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AEmberUE4Character::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	InputComponent->BindAction("LightStance", IE_Pressed, this, &AEmberUE4Character::LightStance);
	InputComponent->BindAction("MediumStance", IE_Pressed, this, &AEmberUE4Character::MediumStance);
	InputComponent->BindAction("HeavyStance", IE_Pressed, this, &AEmberUE4Character::HeavyStance);

	InputComponent->BindAxis("MoveForward", this, &AEmberUE4Character::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AEmberUE4Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AEmberUE4Character::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AEmberUE4Character::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AEmberUE4Character::TouchStarted);
}

void AEmberUE4Character::LightStance()
{
	 if (GEngine)
      {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Light Stance"));
		
			AttachSword();
      }
}

void AEmberUE4Character::MediumStance()
{
 if (GEngine)
      {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Medium Stance"));
			if(SwordModel)
            SwordModel->SetSkeletalMesh(0);		
      }
}

void AEmberUE4Character::HeavyStance()
{
	 if (GEngine)
      {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Heavy Stance"));
			if(SwordModel)
			SwordModel->SetSkeletalMesh(1);		
      }
}

void AEmberUE4Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void AEmberUE4Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AEmberUE4Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AEmberUE4Character::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AEmberUE4Character::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
