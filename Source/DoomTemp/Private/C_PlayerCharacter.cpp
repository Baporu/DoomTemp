// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PlayerCharacter.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"

// Sets default values
AC_PlayerCharacter::AC_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	// Create FPS Camera Component
	FPSCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	// Attach Camera Component to Root Component
	FPSCamComp->SetupAttachment(RootComponent);
	// Set Camera Component's Location
	FPSCamComp->SetRelativeLocation(FVector(-10.0f, 0.0f, 60.0f));
	// Let Pawn Control Camera Rotation
	FPSCamComp->bUsePawnControlRotation = true;
	
	// Create FPS Player Mesh Component
	FPSMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSMeshComp"));
	// Attach Mesh Component to Camera Component
	FPSMeshComp->SetupAttachment(FPSCamComp);

	// Load Skeletal Mesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/SHS/Designs/KrissVector/vector.vector'"));

	// If Skeletal Mesh Loaded
	if (TempMesh.Succeeded()) {
		// Set Loaded Mesh
		FPSMeshComp->SetSkeletalMesh(TempMesh.Object);

		// Set Mesh Component's Location and Rotation
		FPSMeshComp->SetRelativeLocationAndRotation(FVector(-30.0f, 0.0f, -150.0f), FRotator(0.0f, -90.0f, 0.0f));
	}

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
}

// Called when the game starts or when spawned
void AC_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	auto pc = Cast<APlayerController>(Controller);

	if (pc) {
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subsystem) {
			subsystem->AddMappingContext(imc_TPS, 0);
		}
	}
}

// Called every frame
void AC_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// Set Direction as Relative Position
	direction = FTransform(GetControlRotation()).TransformVector(direction);

	// P (Position) = P0 (Current Position) + vt (Velocity * Time)
// 	FVector P0 = GetActorLocation();
// 	FVector vt = direction * walkSpeed * DeltaTime;
// 	FVector P = P0 + vt;
// 	SetActorLocation(P);

	// Call Character Class's Method
	AddMovementInput(direction);

	// Reset Direction
	direction = FVector::ZeroVector;
}

// Called to bind functionality to input
void AC_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (PlayerInput) {
		PlayerInput->BindAction(ia_Turn, ETriggerEvent::Triggered, this, &AC_PlayerCharacter::Turn);
		PlayerInput->BindAction(ia_LookUp, ETriggerEvent::Triggered, this, &AC_PlayerCharacter::LookUp);
		PlayerInput->BindAction(ia_Move, ETriggerEvent::Triggered, this, &AC_PlayerCharacter::Move);
	}
}

void AC_PlayerCharacter::Turn(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerYawInput(value);
}

void AC_PlayerCharacter::LookUp(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerPitchInput(value);
}

void AC_PlayerCharacter::Move(const struct FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();

	direction.X = value.X;
	direction.Y = value.Y;
}

