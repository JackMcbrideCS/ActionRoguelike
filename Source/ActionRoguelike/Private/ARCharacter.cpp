// Fill out your copyright notice in the Description page of Project Settings.


#include "ARCharacter.h"

#include "ARActionComponent.h"
#include "ARAttributeComponent.h"
#include "ARInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

static TAutoConsoleVariable<bool> CVarDebugDrawAim(TEXT("ar.DebugDrawAim"), false, TEXT("Enable debug draws for any aim sweeps."), ECVF_Cheat);

// Sets default values
AARCharacter::AARCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Spring Arm Component");
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera Component");
	CameraComponent->SetupAttachment(SpringArmComponent);

	InteractionComponent = CreateDefaultSubobject<UARInteractionComponent>("Interaction Component");
	AttributeComponent = CreateDefaultSubobject<UARAttributeComponent>("Attribute Component");
	ActionComponent = CreateDefaultSubobject<UARActionComponent>("Action Component");

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AARCharacter::BeginPlay()
{
	Super::BeginPlay();

	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!ensure(PlayerController))
	{
		return;
	}
	
	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	if (!ensure(Subsystem))
	{
		return;
	}

	Subsystem->AddMappingContext(DefaultMappingContext, 0);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &AARCharacter::OnHealthChanged);
}

void AARCharacter::OnMove(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();
	FRotator ControlRotation = Controller->GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;
	const FVector ForwardVector = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(ForwardVector, InputValue.Y);
	AddMovementInput(RightVector, InputValue.X);
}

void AARCharacter::OnSprintStart(const FInputActionValue& Value)
{
	ActionComponent->StartActionByName(this, "Sprint");
}

void AARCharacter::OnSprintStop(const FInputActionValue& Value)
{
	ActionComponent->StopActionByName(this, "Sprint");
}

void AARCharacter::OnLook(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();
	AddControllerYawInput(InputValue.X);
	AddControllerPitchInput(InputValue.Y);
}

void AARCharacter::OnPrimaryAttack(const FInputActionValue& Value)
{
	ActionComponent->StartActionByName(this, "PrimaryAttack");
}

void AARCharacter::OnSecondaryAttack(const FInputActionValue& Value)
{
	ActionComponent->StartActionByName(this, "SecondaryAttack");
}

void AARCharacter::OnJump(const FInputActionValue& Value)
{
	Jump();
}

void AARCharacter::OnInteract(const FInputActionValue& Value)
{
	InteractionComponent->PrimaryInteract();
}

void AARCharacter::OnDodge(const FInputActionValue& Value)
{
	ActionComponent->StartActionByName(this, "DodgeAttack");
}

void AARCharacter::OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComponent, float NewHealth,
	float Delta)
{
	if (Delta >= 0)
	{
		return;
	}
	
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("HitTime"), GetWorld()->GetTimeSeconds());
	if (AttributeComponent->IsAlive())
	{
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	DisableInput(PlayerController);
}

bool AARCharacter::AimTrace(FHitResult& OutHit, const float TraceLength, const FCollisionObjectQueryParams& ObjectQueryParams) const
{
	const FVector TraceStart = CameraComponent->GetComponentLocation();
	const FVector TraceDirection = CameraComponent->GetComponentRotation().Vector() * 1000.0f;
	const FVector TraceEnd = TraceStart + TraceDirection * TraceLength;
	const bool bHit = GetWorld()->LineTraceSingleByObjectType(OutHit, TraceStart, TraceEnd, ObjectQueryParams);
	if (CVarDebugDrawAim.GetValueOnGameThread())
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, bHit ? FColor::Green : FColor::Red, false, 2.0f, 0, 2.0f);
	}
	return bHit;
}

bool AARCharacter::AimSweep(TArray<FHitResult>& OutHits, const float TraceLength,
	const FCollisionObjectQueryParams& ObjectQueryParams, const FCollisionShape& CollisionShape) const
{
	const FVector TraceStart = CameraComponent->GetComponentLocation();
	const FVector TraceDirection = CameraComponent->GetComponentRotation().Vector() * 1000.0f;
	const FVector TraceEnd = TraceStart + TraceDirection * TraceLength;
	const bool bHit = GetWorld()->SweepMultiByObjectType(OutHits, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, CollisionShape);

	if (CVarDebugDrawAim.GetValueOnGameThread())
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, bHit ? FColor::Green : FColor::Red, false, 2.0f, 0, 2.0f);
		for (FHitResult HitResult : OutHits)
		{
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, CollisionShape.GetSphereRadius(), 12, FColor::Green, false, 2.0f);
		}
	}
	
	return bHit;
}

void AARCharacter::HealSelf(const float Amount)
{
	AttributeComponent->ApplyHealthChange(this, Amount);
}

// Called every frame
void AARCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugRotationArrows();
}

// Called to bind functionality to input
void AARCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (!ensure(EnhancedInputComponent))
	{
		return;
	}

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AARCharacter::OnMove);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AARCharacter::OnLook);
	EnhancedInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Started, this, &AARCharacter::OnPrimaryAttack);
	EnhancedInputComponent->BindAction(SecondaryAttackAction, ETriggerEvent::Started, this, &AARCharacter::OnSecondaryAttack);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AARCharacter::Jump);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AARCharacter::OnInteract);
	EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &AARCharacter::OnDodge);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AARCharacter::OnSprintStart);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AARCharacter::OnSprintStop);
}

void AARCharacter::DrawDebugRotationArrows() const
{
	constexpr float DrawScale = 100.0f;
	constexpr float Thickness = 5.0f;
	constexpr float Length = 100.0f;
	constexpr float OffsetDistance = 100.0f;

	const FVector LineOffset = GetActorRightVector() * OffsetDistance;
	const FVector LineStart = GetActorLocation() + LineOffset;
	
	const FVector PlayerLine = GetActorForwardVector() * Length;
	const FVector PlayerLineEnd = LineStart + PlayerLine;
	DrawDebugDirectionalArrow(GetWorld(), LineStart, PlayerLineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);
	
	const FVector ControllerLine = GetControlRotation().Vector() * Length;
	const FVector ControllerLineEnd = LineStart + ControllerLine;
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerLineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}

