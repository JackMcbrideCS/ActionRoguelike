// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARAICharacter.h"

#include "AIController.h"
#include "Actions/ARActionComponent.h"
#include "Attributes/ARAttributeComponent.h"
#include "ARCharacter.h"
#include "ARPlayerState.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "UI/ARWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawPlayerSpotted(TEXT("ar.DebugDrawPlayerSpotted"), false, TEXT("Enable debug draws spotting the player."), ECVF_Cheat);

// Sets default values
AARAICharacter::AARAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("Pawn Sensing Component");
	AttributeComponent = CreateDefaultSubobject<UARAttributeComponent>("Attribute Component");
	ActionComponent = CreateDefaultSubobject<UARActionComponent>("Action Component");
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	CreditGainOnKill = 10;
}

void AARAICharacter::BeginPlay()
{
	Super::BeginPlay();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AARAICharacter::OnPawnSeen);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &AARAICharacter::OnHealthChanged);
}

void AARAICharacter::OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComponent, float NewHealth,
	float Delta)
{
	if (Delta >= 0.0f)
	{
		return;
	}

	if (InstigatorActor == this)
	{
		return;
	}
	
	const AAIController* AIController = Cast<AAIController>(GetController());
	if (!ensure(AIController))
	{
		return;
	}
	
	SetTargetActor(InstigatorActor);
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("HitTime"), GetWorld()->GetTimeSeconds());

	if (!HealthBarWidget)
	{
		HealthBarWidget = CreateWidget<UARWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
		if (HealthBarWidget)
		{
			HealthBarWidget->AttachedActor = this;
			HealthBarWidget->AddToViewport();
		}	
	}

	if (AttributeComponent->IsAlive())
	{
		return;
	}

	AIController->GetBrainComponent()->StopLogic(TEXT("Killed"));
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	AARCharacter* InstigatorPlayer = Cast<AARCharacter>(InstigatorActor);
	if (InstigatorPlayer)
	{
		AARPlayerState* InstigatorPlayerState = InstigatorPlayer->Controller->GetPlayerState<AARPlayerState>();
		InstigatorPlayerState->GainCredits(CreditGainOnKill);
	}
	
	SetLifeSpan(10.0f);
}

void AARAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!ensure(AIController))
	{
		return;
	}
	
	SetTargetActor(Pawn);
}

void AARAICharacter::SetTargetActor(AActor* NewTarget) const
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!ensure(AIController))
	{
		return;
	}
	
	AIController->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), NewTarget);

	if (CVarDebugDrawPlayerSpotted.GetValueOnGameThread())
	{
		DrawDebugString(GetWorld(), GetActorLocation(), TEXT("Player Spotted!"), nullptr, FColor::Red, 4.0f, true);
	}
}
