// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "C_GunSkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AC_PlayerCharacter::AC_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Set Character Default Mesh's Location
	GetMesh()->SetRelativeLocation(FVector(-50.0, 10.0, 40.0));

	// Create FPS Camera Component
	FPSCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	// Attach Camera Component to Root Component
	FPSCamComp->SetupAttachment(RootComponent);
	// Set Camera's Width
	FPSCamComp->OrthoWidth = 1920.0f;
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

	// Update Yaw Only
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Allow Double Jump
	JumpMaxCount = 2;

	// Set Capsule Collider's Radius and Height
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(55.0f);
	// Set Collision Preset
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
}

// Called when the game starts or when spawned
void AC_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	auto pc = Cast<APlayerController>(Controller);

	if (pc) {
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subsystem) {
			subsystem->AddMappingContext(IMC_FPS, 0);
		}
	}

	for (int32 i = 0; i < 1; i++) {
		_Guns[i] = NewObject<UC_GunSkeletalMeshComponent>(this, Guns[i], FName(TEXT("GunMesh"), i));
		
		if (_Guns[i] != nullptr) {
			_Guns[i]->RegisterComponent();
			_Guns[i]->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			_Guns[i]->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

			_Guns[i]->SetRelativeLocation(FVector(30.0, 30.0, 10.0));
			_Guns[i]->SetRelativeScale3D(FVector(0.1));
		}
	}
}

// Called every frame
void AC_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	PlayerMove();
	ResetDashCount(DeltaTime);
}

// Called to bind functionality to input
void AC_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (PlayerInput) {
		PlayerInput->BindAction(IA_LookUp, ETriggerEvent::Triggered, this, &AC_PlayerCharacter::LookUp);
		PlayerInput->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &AC_PlayerCharacter::Turn);

		PlayerInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AC_PlayerCharacter::Move);

		PlayerInput->BindAction(IA_Jump, ETriggerEvent::Started, this, &AC_PlayerCharacter::InputJump);
		
		PlayerInput->BindAction(IA_Dash, ETriggerEvent::Started, this, &AC_PlayerCharacter::Dash);
		//PlayerInput->BindAction(IA_Move, ETriggerEvent::Completed, this, &AC_PlayerCharacter::ResetDashDir);

		PlayerInput->BindAction(IA_Fire, ETriggerEvent::Started, this, &AC_PlayerCharacter::InputFire);
	}
}

void AC_PlayerCharacter::LookUp(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerPitchInput(value);
}

void AC_PlayerCharacter::Turn(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerYawInput(value);
}

void AC_PlayerCharacter::Move(const struct FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();

	MoveDir.X = value.X;
	MoveDir.Y = value.Y;
}

void AC_PlayerCharacter::InputJump(const struct FInputActionValue& inputValue)
{
	Jump();
}

void AC_PlayerCharacter::PlayerMove()
{
	// Set Movement Direction as Relative Position
	MoveDir = FTransform(GetControlRotation()).TransformVector(MoveDir);

	// Call Character Class's Method
	AddMovementInput(MoveDir);

	// Reset Direction
	MoveDir = FVector::ZeroVector;
}

void AC_PlayerCharacter::Dash(const struct FInputActionValue& inputValue)
{
	if (CurDashCount <= 0)
		return;

	/*
	// Player Doesn't Dash Up or Down
	DashDir.Z = 0;

	// Dash Direction Should Be an Unit Vector
	if (DashDir.X > 0)
		DashDir.X = GetActorForwardVector().X;
	else if (DashDir.X < 0)
		DashDir.X = GetActorForwardVector().X * -1;
	else
		DashDir.X = 0.0;

	if (DashDir.Y > 0)
		DashDir.Y = GetActorRightVector().Y;
	else if (DashDir.Y < 0)
		DashDir.Y = GetActorRightVector().Y * -1;
	else
		DashDir.Y = 0.0;

	// Dash Distance Should Be Same in any Direction
	DashDir.Normalize();
	
	// Dash Function
	FVector dashLocation = GetActorLocation() + DashDir * DashDist;
	SetActorLocation(dashLocation, true);
	*/

	//CurDashCount--;
}

void AC_PlayerCharacter::ResetDashDir(const struct FInputActionValue& inputValue)
{
	DashDir = FVector::ZeroVector;
}

void AC_PlayerCharacter::ResetDashCount(float InDeltaTime)
{
	DashTimer -= InDeltaTime;

	if (DashTimer <= 0.0f) {
		CurDashCount = MaxDashCount;

		DashTimer = DashCoolTime;
	}
}

void AC_PlayerCharacter::InputFire(const struct FInputActionValue& inputValue)
{
	_Guns[0]->InputFire();
}

