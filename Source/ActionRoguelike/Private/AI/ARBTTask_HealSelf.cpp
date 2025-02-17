// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARBTTask_HealSelf.h"

#include "AIController.h"
#include "Attributes/ARAttributeComponent.h"

EBTNodeResult::Type UARBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!ensure(AIController))
	{
		return EBTNodeResult::Failed;
	}

	APawn* AIPawn = AIController->GetPawn();
	if (!ensure(AIPawn))
	{
		return EBTNodeResult::Failed;
	}
	
	UARAttributeComponent* AttributeComponent = UARAttributeComponent::GetAttributes(AIPawn);
	if (!ensure(AttributeComponent))
	{
		return EBTNodeResult::Failed;
	}

	AttributeComponent->ApplyHealthChange(AIPawn, HealthGain);
	return EBTNodeResult::Succeeded;
}
