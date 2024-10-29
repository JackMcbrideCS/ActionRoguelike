// Fill out your copyright notice in the Description page of Project Settings.


#include "ARCharacter.h"

#include "ARInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AARCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}	
}

void AARCharacter::OnMove(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();
	const FRotator ControlRotation = Controller->GetControlRotation();
	const FVector ForwardVector = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(ForwardVector, InputValue.Y);
	AddMovementInput(RightVector, InputValue.X);
}

void AARCharacter::OnLook(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();
	AddControllerYawInput(InputValue.X);
	AddControllerPitchInput(InputValue.Y);
}

void AARCharacter::OnPrimaryAttack(const FInputActionValue& Value)
{
	PlayAnimMontage(AttackAnimMontage);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AARCharacter::PrimaryAttack_TimerElapsed, 0.2f);
}

void AARCharacter::PrimaryAttack_TimerElapsed()
{
	const bool bSocketExists = GetMesh()->DoesSocketExist(PrimaryAttackSocket);
	if (!bSocketExists)
	{
		UE_LOG(LogTemp, Warning, TEXT("Primary Attack Socket does not exist [%s]."), *PrimaryAttackSocket.ToString())
	}
	
	const FVector SpawnLocation = bSocketExists ? GetMesh()->GetSocketLocation(PrimaryAttackSocket) : GetActorLocation();

	FHitResult Hit;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	const bool bBlockingHit = AimTrace(Hit, 1000.0f, ObjectQueryParams);
	
	const FVector LookAtPosition = bBlockingHit ? Hit.ImpactPoint : Hit.TraceEnd;
	const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, LookAtPosition);
	const FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = this;
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParameters);
}

void AARCharacter::OnJump(const FInputActionValue& Value)
{
	Jump();
}

void AARCharacter::OnInteract(const FInputActionValue& Value)
{
	InteractionComponent->PrimaryInteract();
}

bool AARCharacter::AimTrace(FHitResult& OutHit, const float TraceLength, const FCollisionObjectQueryParams& ObjectQueryParams) const
{
	const FVector TraceStart = CameraComponent->GetComponentLocation();
	const FVector TraceDirection = CameraComponent->GetComponentRotation().Vector() * 1000.0f;
	const FVector TraceEnd = TraceStart + TraceDirection * TraceLength;
	return GetWorld()->LineTraceSingleByObjectType(OutHit, TraceStart, TraceEnd, ObjectQueryParams);
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

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AARCharacter::OnMove);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AARCharacter::OnLook);
		EnhancedInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Started, this, &AARCharacter::OnPrimaryAttack);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AARCharacter::Jump);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AARCharacter::OnInteract);
	}
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

