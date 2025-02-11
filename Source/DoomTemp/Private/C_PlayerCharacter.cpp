// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "C_GunSkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "C_PlasmaGun.h"
#include "C_SniperGun.h"
#include "C_ShotGun.h"

// Sets default values
AC_PlayerCharacter::AC_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Set Character Default Mesh's Location
	GetMesh()->SetRelativeLocation(FVector(-50.0, 10.0, 50.0));

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
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/KrissVector/vector.vector'"));

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

	// Set Gun Mesh Components
	{
		{
			// Create Gun Mesh Component
			PlasmaMesh = CreateDefaultSubobject<UC_PlasmaGun>(TEXT("PlasmaMesh"));
			// Attach Mesh Component to Camera Component
			PlasmaMesh->SetupAttachment(GetMesh());
			// Load Skeletal Mesh
			ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/SHS/Designs/Experimental_Rifle_Coilgun/vector.vector'"));

			// If Skeletal Mesh Loaded
			if (GunMesh.Succeeded()) {
				// Set Loaded Mesh
				PlasmaMesh->SetSkeletalMesh(GunMesh.Object);

				// Set Mesh Component's Location and Rotation
				PlasmaMesh->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));
			}
		}
		{
			// Create Gun Mesh Component
			SniperMesh = CreateDefaultSubobject<UC_SniperGun>(TEXT("SniperMesh"));
			// Attach Mesh Component to Camera Component
			SniperMesh->SetupAttachment(GetMesh());
			// Load Skeletal Mesh
			ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/SV-98_Sniper/source/vector.vector'"));

			// If Skeletal Mesh Loaded
			if (GunMesh.Succeeded()) {
				// Set Loaded Mesh
				SniperMesh->SetSkeletalMesh(GunMesh.Object);

				// Set Mesh Component's Location and Rotation
				SniperMesh->SetRelativeLocationAndRotation(FVector(10.0, 50.0, 10.0), FRotator(0.0, -90.0, 0.0));
				SniperMesh->SetRelativeScale3D(FVector(0.15));
			}
		}
 		{
			// Create Gun Mesh Component
			ShotgunMesh = CreateDefaultSubobject<UC_GunSkeletalMeshComponent>(TEXT("ShotgunMesh"));
			// Attach Mesh Component to Camera Component
			ShotgunMesh->SetupAttachment(GetMesh());
			// Load Skeletal Mesh
			ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/package/Skeleton_mesh/SKM_Shotgun.SKM_Shotgun'"));

			// If Skeletal Mesh Loaded
			if (GunMesh.Succeeded()) {
				// Set Loaded Mesh
				ShotgunMesh->SetSkeletalMesh(GunMesh.Object);

				// Set Mesh Component's Location and Rotation
				ShotgunMesh->SetRelativeLocationAndRotation(FVector(-30.0f, 0.0f, -150.0f), FRotator(0.0f, -90.0f, 0.0f));
			}
 		}
	}

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

	SetWeaponActive(EWeaponType::Plasma, false);
	SetWeaponActive(EWeaponType::Sniper, false);
	//SetWeaponActive(EWeaponType::Shotgun, false);
	SetWeaponActive(mWeaponType, true);

// 	for (int32 i = 0; i < 3; i++) {
// 		if (Guns[i] != nullptr) {
// 			UC_GunSkeletalMeshComponent* gun = NewObject<UC_GunSkeletalMeshComponent>(this, Guns[i].GetDefaultObject()->StaticClass(), Guns[i].GetDefaultObject()->MeshName);
// 
// 			gun->RegisterComponent();
// 			gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
// 			gun->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));
// 		}
// 	}
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
		PlayerInput->BindAction(IA_LookUp, ETriggerEvent::Triggered, this, &AC_PlayerCharacter::OnLookUp);
		PlayerInput->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &AC_PlayerCharacter::OnTurn);

		PlayerInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AC_PlayerCharacter::OnMove);

		PlayerInput->BindAction(IA_Jump, ETriggerEvent::Started, this, &AC_PlayerCharacter::OnJump);
		
		PlayerInput->BindAction(IA_Dash, ETriggerEvent::Started, this, &AC_PlayerCharacter::OnDash);
		//PlayerInput->BindAction(IA_Move, ETriggerEvent::Completed, this, &AC_PlayerCharacter::ResetDashDir);

		PlayerInput->BindAction(IA_Fire, ETriggerEvent::Started, this, &AC_PlayerCharacter::OnFire);
		PlayerInput->BindAction(IA_Fire, ETriggerEvent::Completed, this, &AC_PlayerCharacter::OnFire);

		PlayerInput->BindAction(IA_UseMode, ETriggerEvent::Started, this, &AC_PlayerCharacter::OnUseMode);
		PlayerInput->BindAction(IA_UseMode, ETriggerEvent::Completed, this, &AC_PlayerCharacter::OnUseMode);
	}
}

void AC_PlayerCharacter::OnLookUp(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerPitchInput(value);
}

void AC_PlayerCharacter::OnTurn(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerYawInput(value);
}

void AC_PlayerCharacter::OnMove(const struct FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();

	MoveDir.X = value.X;
	MoveDir.Y = value.Y;
}

void AC_PlayerCharacter::OnJump(const struct FInputActionValue& inputValue)
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

void AC_PlayerCharacter::OnDash(const struct FInputActionValue& inputValue)
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

void AC_PlayerCharacter::OnFire(const struct FInputActionValue& inputValue)
{
	bIsFire = !bIsFire;

	switch (mWeaponType)
	{
		case EWeaponType::Plasma:	{Fire_Plasma();}	break;
		case EWeaponType::Sniper:	{Fire_Sniper();}	break;
		case EWeaponType::Shotgun:	{Fire_Shotgun();}	break;
	}
}

void AC_PlayerCharacter::Fire_Plasma()
{
	PlasmaMesh->OnFire();
}


void AC_PlayerCharacter::Fire_Sniper()
{
	SniperMesh->OnFire();
}


void AC_PlayerCharacter::Fire_Shotgun()
{
	ShotgunMesh->OnFire();
}

void AC_PlayerCharacter::OnUseMode(const struct FInputActionValue& inputValue)
{
	switch (mWeaponType)
	{
		case EWeaponType::Plasma:	{ PlasmaMesh->OnUseMode(); }	break;
		case EWeaponType::Sniper:	{ SniperMesh->OnUseMode(); }	break;
		case EWeaponType::Shotgun:	{ ShotgunMesh->OnUseMode(); }	break;
	}
}

void AC_PlayerCharacter::ChangeWeapon(EWeaponType InChangeType)
{
	SetWeaponActive(mWeaponType, false);
	mWeaponType = InChangeType;
	SetWeaponActive(InChangeType, true);
}

void AC_PlayerCharacter::SetWeaponActive(EWeaponType InChangeType, bool InActive)
{
	switch (InChangeType)
	{
		case EWeaponType::Plasma:	{ PlasmaMesh->SetVisibility(InActive); }	break;
		case EWeaponType::Sniper:	{ SniperMesh->SetVisibility(InActive); }	break;
		//case EWeaponType::Shotgun:	{ ShotgunMesh->SetVisibility(InActive); }	break;
	}
}

UCameraComponent* AC_PlayerCharacter::GetCameraComponent()
{
	return FPSCamComp;
}

