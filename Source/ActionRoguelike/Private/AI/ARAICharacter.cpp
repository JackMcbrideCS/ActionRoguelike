// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARAICharacter.h"

#include "AIController.h"
#include "ARAttributeComponent.h"
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
}

void AARAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController)
	{
		return;
	}
	
	AIController->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Pawn);
	DrawDebugString(GetWorld(), GetActorLocation(), TEXT("Player Spotted!"), nullptr, FColor::Red, 4.0f, true);
}
