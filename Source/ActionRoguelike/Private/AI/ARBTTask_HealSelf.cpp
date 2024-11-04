// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARBTTask_HealSelf.h"

#include "AIController.h"
#include "ARAttributeComponent.h"

EBTNodeResult::Type UARBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!ensure(AIController))
	{
		return EBTNodeResult::Failed;
	}

	const APawn* AIPawn = AIController->GetPawn();
	if (!ensure(AIPawn))
	{
		return EBTNodeResult::Failed;
	}
	
	UARAttributeComponent* AttributeComponent = AIPawn->GetComponentByClass<UARAttributeComponent>();
	if (!ensure(AttributeComponent))
	{
		return EBTNodeResult::Failed;
	}

	AttributeComponent->ApplyHealthChange(HealthGain);
	return EBTNodeResult::Succeeded;
}
