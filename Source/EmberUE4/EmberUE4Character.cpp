// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "EmberUE4.h"
#include "EmberUE4Character.h"

//////////////////////////////////////////////////////////////////////////
// AEmberUE4Character

//////////////////////////////////////////////////////////////////////////
// Constructor
AEmberUE4Character::AEmberUE4Character(const class FPostConstructInitializeProperties &PCIP)
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

    static ConstructorHelpers::FObjectFinder<UBlueprint> SwordBlueprint(TEXT("Blueprint'/Game/Blueprints/SwordModelBlueprint.SwordModelBlueprint'"));
    if (SwordBlueprint.Object != NULL)
    {
        SwordBlueprint_GeneratedClass = (UClass *)SwordBlueprint.Object->GeneratedClass;
    }
}

//////////////////////////////////////////////////////////////////////////
// Additional Constructing Functions
void AEmberUE4Character::BeginPlay()
{
    Super::BeginPlay();
    SetupVariables();
    AttachSword();
}

void AEmberUE4Character::SetupVariables()
{
    PrimaryActorTick.bCanEverTick = true;
    DrawSwordDebug = false;
    DrawNewSwordDebug = false;
    bPlayerIsAttacking = false;
    bLeftClickPressed = false;
}

// Attaches sword to model. Runs in BeginPlay
void AEmberUE4Character::AttachSword()
{
    UWorld *const World = GetWorld();
    if (!SwordModel)
    {
        SwordModel = World->SpawnActor<ASwordModel>(SwordBlueprint_GeneratedClass);
        SwordModel->AttachRootComponentTo(this->Mesh, FName(TEXT("RIGHT_HAND_ATTACH")));
    }
    ChangeStance(1);
}
//////////////////////////////////////////////////////////////////////////
// System / Per Tick Functions

void AEmberUE4Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(bLeftClickPressed) SwordDebug();

    SwordCalculations(DeltaTime);
}

void AEmberUE4Character::SwordCalculations(float DeltaTime)
{
    if(bPlayerIsAttacking)   
    {

    }
}

void AEmberUE4Character::SwordDebug()
{
    // if (SwordModel && DrawSwordDebug)
    // {
    //     FVector socketLocation = SwordModel->StartSocket();
    //     FVector socketLocation2 = SwordModel->EndSocket();
    //     DrawDebugLine(GetWorld(), socketLocation,  socketLocation2,  FColor(255, 0, 0),  false, 2, 0,  5 );
    // }

    // if (SwordModel && DrawNewSwordDebug)
    SwordModel ? SwordModel->TraceSword() : AttachSword();
}

//////////////////////////////////////////////////////////////////////////
// Inputs

void AEmberUE4Character::SetupPlayerInputComponent(class UInputComponent *InputComponent)
{
    // Set up gameplay key bindings
    check(InputComponent);
    InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

    InputComponent->BindAction("LightStance", IE_Pressed, this, &AEmberUE4Character::LightStance);
    InputComponent->BindAction("MediumStance", IE_Pressed, this, &AEmberUE4Character::MediumStance);
    InputComponent->BindAction("HeavyStance", IE_Pressed, this, &AEmberUE4Character::HeavyStance);

    InputComponent->BindAction("W_Pressed", IE_Pressed, this, &AEmberUE4Character::W_Pressed);
    InputComponent->BindAction("A_Pressed", IE_Pressed, this, &AEmberUE4Character::A_Pressed);
    InputComponent->BindAction("S_Pressed", IE_Pressed, this, &AEmberUE4Character::S_Pressed);
    InputComponent->BindAction("D_Pressed", IE_Pressed, this, &AEmberUE4Character::D_Pressed);

    InputComponent->BindAction("W_Pressed", IE_Released, this, &AEmberUE4Character::W_Pressed);
    InputComponent->BindAction("A_Pressed", IE_Released, this, &AEmberUE4Character::A_Pressed);
    InputComponent->BindAction("S_Pressed", IE_Released, this, &AEmberUE4Character::S_Pressed);
    InputComponent->BindAction("D_Pressed", IE_Released, this, &AEmberUE4Character::D_Pressed);

    InputComponent->BindAction("LeftClick", IE_Released, this, &AEmberUE4Character::LeftClickPressed);
    InputComponent->BindAction("LeftClick", IE_Pressed, this, &AEmberUE4Character::LeftClickPressed);

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

void AEmberUE4Character::LeftClickPressed()
{
    bLeftClickPressed = !bLeftClickPressed;
	/*
        Is left clicked being pressed?
            If it is, does the character actually have a sword?
                If he does, reset the sword to prepare it for attack
                If he doesn't, attach a sword to him
        If left click isn't pressed, do nothing
	*/
    bLeftClickPressed ? SwordModel ? SwordModel->ResetSword() : AttachSword() : false;

    PrepareAttack();
}

void AEmberUE4Character::W_Pressed()
{
    Keyboard_Hook_W = !Keyboard_Hook_W;
    GetForwardBackResultant();
}
void AEmberUE4Character::A_Pressed()
{
    Keyboard_Hook_A = !Keyboard_Hook_A;
    GetLeftRightResultant();
}
void AEmberUE4Character::S_Pressed()
{
    Keyboard_Hook_S = !Keyboard_Hook_S;
    GetForwardBackResultant();
}
void AEmberUE4Character::D_Pressed()
{
    Keyboard_Hook_D = !Keyboard_Hook_D;
    GetLeftRightResultant();
}

void AEmberUE4Character::GetLeftRightResultant()
{
    LR_Result = Keyboard_Hook_D - Keyboard_Hook_A;
}
void AEmberUE4Character::GetForwardBackResultant()
{
    FB_Result = Keyboard_Hook_W - Keyboard_Hook_S;
}
//////////////////////////////////////////////////////////////////////////
// Functions

void AEmberUE4Character::PrepareAttack()
{
    int iTotalKeyFlag = 0;

    iTotalKeyFlag = Keyboard_Hook_W + Keyboard_Hook_A + Keyboard_Hook_S + Keyboard_Hook_D;

    // switch(iTotalKeyFlag)
    //     {
    //         //no keys pressed
    //         case 0:
    //             forwardAttack();
    //         break;

    //         //one key pressed 
    //         case 1:
    //             if((savedByteDirection[0] ^ 1) == 0 ) forwardAttack(); //W
    //             if((savedByteDirection[1] ^ 1) == 0 ) leftAttack(); //A
    //             if((savedByteDirection[2] ^ 1) == 0 ) backAttack(); //S
    //             if((savedByteDirection[3] ^ 1) == 0 ) rightAttack(); //D
    //         break;

    //         //two keys pressed 
    //         case 2:
    //             if((savedByteDirection[0] ^ 1) == 0 && (savedByteDirection[1] ^ 1) == 0 ) forwardLeftAttack(); //W+A
    //             else if((savedByteDirection[0] ^ 1) == 0 && (savedByteDirection[3] ^ 1) == 0 ) forwardRightAttack(); //W+D
    //             else if((savedByteDirection[2] ^ 1) == 0 && (savedByteDirection[1] ^ 1) == 0 ) backLeftAttack(); //S+A
    //             else if((savedByteDirection[2] ^ 1) == 0 && (savedByteDirection[3] ^ 1) == 0 ) backRightAttack(); //S+D

    //             //for keys W + S and A + D
    //             else forwardAttack();

    //         break;

    //         //3-4 keys pressed
    //         case 3:
    //         case 4:
    //             forwardAttack();
    //         break;
        
    //     }
}

//////////////////////////////////////////////////////////////////////////
// Stances

void AEmberUE4Character::ChangeStance(int32 Index)
{
    CurrentStance = Index;

    SwordModel ? SwordModel->SetSkeletalMesh(CurrentStance) : AttachSword();
}

void AEmberUE4Character::LightStance()
{
    EmberUtilities::Debug("Light Stance");
    ChangeStance(0);
}

void AEmberUE4Character::MediumStance()
{
    EmberUtilities::Debug("Medium Stance");
    ChangeStance(1);
    DrawSwordDebug = !DrawSwordDebug;
}

void AEmberUE4Character::HeavyStance()
{
    EmberUtilities::Debug("Heavy Stance");
    ChangeStance(2);
    DrawNewSwordDebug = !DrawNewSwordDebug;
}

//////////////////////////////////////////////////////////////////////////
// Movements

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

/////////////////////////////////////////////////////////////////////////