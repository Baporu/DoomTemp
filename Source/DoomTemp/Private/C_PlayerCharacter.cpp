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
#include "Enemy/C_Enemy.h"
#include "C_PlayerAnimInstance.h"
#include "Components/BoxComponent.h"

// Sets default values
AC_PlayerCharacter::AC_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Set Character Class Default Mesh's Locaiton (Meaning Gun Meshes' Default Location)
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
		FPSMeshComp->SetRelativeLocationAndRotation(FVector(20.0f, 0.0f, -150.0f), FRotator(0.0f, -90.0f, 0.0f));
	}

	MeleeComp = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeComp"));
	
	MeleeComp->SetupAttachment(FPSCamComp);
	MeleeComp->SetRelativeLocation(FVector(215.0, 0.0, 0.0));
	MeleeComp->SetRelativeScale3D(FVector(5.0, 3.0, 1.0));

	MeleeComp->SetCollisionProfileName(TEXT("PlayerAttack"));
	MeleeComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeleeComp->OnComponentBeginOverlap.AddDynamic(this, &AC_PlayerCharacter::OnMeleeOverlap);

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

	// Set Mesh Component For Each Guns
	{
		// Plasma Gun Mesh
		{
			// Create Gun Mesh Component
			PlasmaMesh = CreateDefaultSubobject<UC_PlasmaGun>(TEXT("PlasmaMesh"));
			// Attach Mesh Component to Camera Component
			PlasmaMesh->SetupAttachment(FPSMeshComp);
			// Load Skeletal Mesh
			ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/textured_gun/vector.vector'"));

			// If Skeletal Mesh Loaded
			if (GunMesh.Succeeded()) {
				// Set Loaded Mesh
				PlasmaMesh->SetSkeletalMesh(GunMesh.Object);

				// Set Mesh Component's Rotation and Scale
				PlasmaMesh->SetRelativeLocation(FVector(-42.0, -3.0, 120.0));
				PlasmaMesh->SetRelativeScale3D(FVector(0.08));
			}
		}

		// Sniper Gun Mesh
		{
			// Create Gun Mesh Component
			SniperMesh = CreateDefaultSubobject<UC_SniperGun>(TEXT("SniperMesh"));
			// Attach Mesh Component to Camera Component
			SniperMesh->SetupAttachment(FPSMeshComp);
			// Load Skeletal Mesh
			ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/SV-98_Sniper/source/vector.vector'"));

			// If Skeletal Mesh Loaded
			if (GunMesh.Succeeded()) {
				// Set Loaded Mesh
				SniperMesh->SetSkeletalMesh(GunMesh.Object);

				// Set Mesh Component's Location and Rotation
				SniperMesh->SetRelativeLocation(FVector(-56.0, 5.5, 140.0));
				SniperMesh->SetRelativeScale3D(FVector(0.15));
			}
		}

		// Shotgun Mesh
 		{
			// Create Gun Mesh Component
			ShotgunMesh = CreateDefaultSubobject<UC_ShotGun>(TEXT("ShotgunMesh"));
			// Attach Mesh Component to Camera Component
			ShotgunMesh->SetupAttachment(FPSMeshComp);
			// Load Skeletal Mesh
			ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/package/Skeleton_mesh/SKM_Shotgun.SKM_Shotgun'"));

			// If Skeletal Mesh Loaded
			if (GunMesh.Succeeded()) {
				// Set Loaded Mesh
				ShotgunMesh->SetSkeletalMesh(GunMesh.Object);

				// Set Mesh Component's Location and Rotation
				ShotgunMesh->SetRelativeLocation(FVector(-10.0, 27.0, 136.0));
				ShotgunMesh->SetRelativeScale3D(FVector(1.5));
			}
 		}
	}

}

// Called when the game starts or when spawned
void AC_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	PlasmaMesh->me = this;
	PlasmaMesh->FPSCam = FPSCamComp;
	SniperMesh->me = this;
	SniperMesh->FPSCam = FPSCamComp;
	ShotgunMesh->me = this;
	ShotgunMesh->FPSCam = FPSCamComp;

	Anim = Cast<UC_PlayerAnimInstance>(FPSMeshComp->GetAnimInstance());

	// Get Player Controller and Map with IMC
	auto pc = Cast<APlayerController>(Controller);

	if (pc) {
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subsystem) {
			subsystem->AddMappingContext(IMC_FPS, 0);
		}
	}

	// Disable All Weapon Meshes, and then Enable Current Weapon Mesh
	SetWeaponActive(EWeaponType::Plasma, false);
	SetWeaponActive(EWeaponType::Sniper, false);
	SetWeaponActive(EWeaponType::Shotgun, false);
	SetWeaponActive(mWeaponType, true);

	// Set Fire Rate
	SetFireRate(GetCurrentGun()->GetFireRate());
	FireTimer = FireRate;

	// Set Stats
	CurrentHP = MaxHP;

	// Set Fuel Timer
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AC_PlayerCharacter::OnFuelTime, (FuelTime / 2), false);

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
	FireTimer += DeltaTime;
	PlayerFire();
	ResetDashCount();
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
		
		PlayerInput->BindAction(IA_Fire, ETriggerEvent::Started, this, &AC_PlayerCharacter::OnFire);
		PlayerInput->BindAction(IA_Fire, ETriggerEvent::Completed, this, &AC_PlayerCharacter::OnFire);

		PlayerInput->BindAction(IA_UseMode, ETriggerEvent::Started, this, &AC_PlayerCharacter::OnUseMode);
		PlayerInput->BindAction(IA_UseMode, ETriggerEvent::Completed, this, &AC_PlayerCharacter::OnUseMode);

		PlayerInput->BindAction(IA_ChangeWeapon, ETriggerEvent::Started, this, &AC_PlayerCharacter::OnChangeWeapon);

		PlayerInput->BindAction(IA_Punch, ETriggerEvent::Started, this, &AC_PlayerCharacter::OnPunch);
		PlayerInput->BindAction(IA_Saw, ETriggerEvent::Started, this, &AC_PlayerCharacter::OnSaw);
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

	DashDir = MoveDir;
	DashDir.Z = 0.0;

	// Reset Direction
	MoveDir = FVector::ZeroVector;
}

void AC_PlayerCharacter::OnDash(const struct FInputActionValue& inputValue)
{
	if (CurDashCount <= 0 || DashDir == FVector::ZeroVector)
		return;

	DashDir.Normalize();
	UE_LOG(LogTemp, Warning, TEXT("DashDir X: %f, Y: %f, Z: %f"), DashDir.X, DashDir.Y, DashDir.Z);

	SetActorLocation(GetActorLocation() + DashDir * DashDistance, true);

	DashDir = FVector::ZeroVector;
	CurDashCount--;
}

void AC_PlayerCharacter::ResetDashCount()
{
	DashTimer -= GetWorld()->GetDeltaSeconds();

	if (DashTimer <= 0.0f) {
		CurDashCount = MaxDashCount;

		DashTimer = DashCoolTime;
	}
}

void AC_PlayerCharacter::OnFire(const struct FInputActionValue& inputValue)
{
	if (mWeaponType == EWeaponType::Shotgun && ShotgunMesh->bUsingMode == false) {
		bIsFire = false;
		bShotgun = !bShotgun;

		if (bShotgun)
			if (FireTimer >= FireRate) {
				Fire_Shotgun();

				FireTimer = 0.0f;
			}
	}
	else
		bIsFire = !bIsFire;
}

void AC_PlayerCharacter::PlayerFire()
{
	if (!bIsFire)
		return;

	if (FireTimer >= FireRate) {

		switch (mWeaponType)
		{
			case EWeaponType::Plasma: { Fire_Plasma(); }	break;
			case EWeaponType::Sniper: { Fire_Sniper(); }	break;
			case EWeaponType::Shotgun: { Fire_Shotgun(); }	break;
		}

		FireTimer = 0.0f;
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

void AC_PlayerCharacter::OnChangeWeapon(const struct FInputActionValue& inputValue)
{
	switch (mWeaponType)
	{
		case EWeaponType::Plasma:	{ ChangeWeapon(EWeaponType::Sniper); }	break;
		case EWeaponType::Sniper:	{ ChangeWeapon(EWeaponType::Shotgun); }	break;
		case EWeaponType::Shotgun:	{ ChangeWeapon(EWeaponType::Plasma); }	break;
	}
}

void AC_PlayerCharacter::ChangeWeapon(EWeaponType InChangeType)
{
	// Reset Mode Use
	switch (mWeaponType)
	{
		case EWeaponType::Plasma: { PlasmaMesh->OnGunChanged(); }	break;
		case EWeaponType::Sniper: { SniperMesh->OnGunChanged(); }	break;
		case EWeaponType::Shotgun: { ShotgunMesh->OnGunChanged(); }	break;
	}

	// Change Gun Mesh
	SetWeaponActive(mWeaponType, false);
	mWeaponType = InChangeType;
	SetWeaponActive(InChangeType, true);
	
	// Change Fire Rate
	FireRate = GetCurrentGun()->GetFireRate();
}

void AC_PlayerCharacter::SetWeaponActive(EWeaponType InChangeType, bool InActive)
{
	switch (InChangeType)
	{
		case EWeaponType::Plasma:	{ PlasmaMesh->SetVisibility(InActive); }	break;
		case EWeaponType::Sniper:	{ SniperMesh->SetVisibility(InActive); }	break;
		case EWeaponType::Shotgun:	{ ShotgunMesh->SetVisibility(InActive); }	break;
	}
}

void AC_PlayerCharacter::PlayerHit(int32 InDamage)
{
	CurrentHP -= InDamage;
	UE_LOG(LogTemp, Warning, TEXT("Player Take Damage"));

	if (CurrentHP <= 0) {
		GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Red, TEXT("Player Dead"));
		UE_LOG(LogTemp, Log, TEXT("Player Dead"));
	}
}

void AC_PlayerCharacter::SetFireRate(float InFireRate)
{
	FireRate = InFireRate;
}

void AC_PlayerCharacter::OnPunch(const struct FInputActionValue& inputValue)
{
	// Deactivate Weapon Mesh
	SetWeaponActive(mWeaponType, false);

	// Dash to Target
	MeleeDash();

	// Start Punch Animation
	Anim->bIsPunching = true;

	if (!MeleeTarget)
		return;

	if (MeleeTarget->bIsStaggered)
		MeleeTarget->OnDamageProcess(10000, EAttackType::GloryKill);
	else
		MeleeTarget->OnDamageProcess(MeleeDamage, EAttackType::Fist);
	
}

void AC_PlayerCharacter::OnPunchEnd()
{
	// Activate Weapon Mesh
	SetWeaponActive(mWeaponType, true);

	MeleeTarget = nullptr;
}

void AC_PlayerCharacter::OnSaw(const struct FInputActionValue& inputValue)
{
	if (CurrentFuel <= 0)
		return;

	else
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AC_PlayerCharacter::OnFuelTime, FuelTime, false);

	// Deactivate Weapon Mesh
	SetWeaponActive(mWeaponType, false);

	// Dash to Target
	MeleeDash();

	// Start Punch Animation
	Anim->bIsPunching = true;

	if (!MeleeTarget)
		return;

	MeleeTarget->OnDamageProcess(10000, EAttackType::Chainsaw);
}

UCameraComponent* AC_PlayerCharacter::GetCameraComponent()
{
	return FPSCamComp;
}

UC_GunSkeletalMeshComponent* AC_PlayerCharacter::GetCurrentGun()
{
	switch (mWeaponType) {
		case EWeaponType::Plasma:	{ return PlasmaMesh; }
		case EWeaponType::Sniper:	{ return SniperMesh; }
		case EWeaponType::Shotgun:	{ return ShotgunMesh; }
	}

	return ShotgunMesh;
}

void AC_PlayerCharacter::OnMeleeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AC_Enemy* enemy = Cast<AC_Enemy>(OtherActor);

	if (!enemy)
		return;

	// If There is No Target, Set Target
	if (!MeleeTarget) {
		MeleeTarget = enemy;
	}
	// If There is Target,
	else {
		// Get Distances
		double curDist = FVector::Dist(GetActorLocation(), MeleeTarget->GetActorLocation());
		double newDist = FVector::Dist(GetActorLocation(), enemy->GetActorLocation());

		// Compare Distance, Then Set Target
		if (newDist < curDist)
			MeleeTarget = enemy;
	}
}

void AC_PlayerCharacter::MeleeDash()
{
	// Enable Target Finding Colldier Component
	MeleeComp->SetVisibility(true);
	MeleeComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// Disable Target Finding Collider Component
	MeleeComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeleeComp->SetVisibility(false);

	if (!MeleeTarget) {
		// Show Target Not Found UI
		return;
	}

	UCapsuleComponent* capsuleComp = Cast<UCapsuleComponent>(RootComponent);

	if (capsuleComp) {
		capsuleComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
		SetActorLocation(MeleeTarget->GetActorLocation(), true);
		capsuleComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);
	}

	else {
		UE_LOG(LogTemp, Error, TEXT("Capsule Component Not Found!!!"));
	}
}

void AC_PlayerCharacter::OnFuelTime()
{
	CurrentFuel++;

	if (CurrentFuel < MaxFuel)
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AC_PlayerCharacter::OnFuelTime, FuelTime, false);

}

