// Fill out your copyright notice in the Description page of Project Settings.


#include "LavaCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ALavaCharacter::ALavaCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Stops character from rotating when camera turns
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	// Configure character movement to turn toward movement direction
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // Distance behind character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach camera to end of boom
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	// Setups the movement tuning
	SetupMovementTuning();
}

// Called when the game starts or when spawned
void ALavaCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(GEngine != nullptr);
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(ThirdPersonContext, 0);
		}
	}
}

// Called every frame
void ALavaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALavaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind Move
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALavaCharacter::Move);
		
		// Bind Jump
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ALavaCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ALavaCharacter::StopJumping);
		
		// Bind Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALavaCharacter::Look);
	}
}

void ALavaCharacter::SetupMovementTuning()
{
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	if (Movement)
	{
		Movement->JumpZVelocity = JumpZVelocityTuning;
		Movement->AirControl = AirControlTuning;
		Movement->GravityScale = GravityScaleTuning;
	}
	// Changes Jump Count to 2
	JumpMaxCount = 2;
}

void ALavaCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();
	
	// Translates input to movement in game
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardDirection, MovementValue.Y);
		AddMovementInput(RightDirection, MovementValue.X);
		
	}
}

void ALavaCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	
	if (Controller)
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}
