// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARAICharacter.h"

#include "AIController.h"
#include "ARAttributeComponent.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AARAICharacter::AARAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("Pawn Sensing Component");
	AttributeComponent = CreateDefaultSubobject<UARAttributeComponent>("Attribute Component");
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

	if (AttributeComponent->IsAlive())
	{
		return;
	}
	
	const AAIController* AIController = Cast<AAIController>(GetController());
	if (!ensure(AIController))
	{
		return;
	}

	AIController->GetBrainComponent()->StopLogic(TEXT("Killed"));
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	
	SetLifeSpan(10.0f);
}

void AARAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!ensure(AIController))
	{
		return;
	}
	
	AIController->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Pawn);
	DrawDebugString(GetWorld(), GetActorLocation(), TEXT("Player Spotted!"), nullptr, FColor::Red, 4.0f, true);
}
