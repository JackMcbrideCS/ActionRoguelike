// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARBTService_HealthThreshold.h"

#include "AIController.h"
#include "ARAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void UARBTService_HealthThreshold::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!ensure(AIController))
	{
		return;
	}

	const APawn* AIPawn = AIController->GetPawn();
	if (!ensure(AIPawn))
	{
		return;
	}

	const UARAttributeComponent* AttributeComponent = UARAttributeComponent::GetAttributes(AIPawn);
	if (!ensure(AttributeComponent))
	{
		return;
	}

	bool bCondition;
	const float Health = AttributeComponent->GetHealth();
	switch (Mode)
	{
	case EBTServiceHealthThreshold::Greater:
		bCondition = Health > Threshold;
		break;
	case EBTServiceHealthThreshold::GreaterEqual:
		bCondition = Health >= Threshold;
		break;
	case EBTServiceHealthThreshold::Less:
		bCondition = Health < Threshold;
		break;
	case EBTServiceHealthThreshold::LessEqual:
		bCondition = Health <= Threshold;
		break;
	default:
		ensure(false);
		return;
	}

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!ensure(BlackboardComponent))
	{
		return;
	}

	BlackboardComponent->SetValueAsBool(HealthThresholdKey.SelectedKeyName, bCondition);
}
