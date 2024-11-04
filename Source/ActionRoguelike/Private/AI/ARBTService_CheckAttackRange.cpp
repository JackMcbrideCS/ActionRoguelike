// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UARBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!ensure(AIController))
	{
		return;
	}

	const APawn* AIPawn = AIController->GetPawn();
	if (!ensure(AIPawn))
	{
		return;
	}
	
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!ensure(BlackboardComponent))
	{
		return;
	}

	const AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetActorRangeKey.SelectedKeyName));
	if (!TargetActor)
	{
		return;
	}

	const float Distance = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
	const bool bInRange = Distance < 2000.0f;
	if (!bInRange)
	{
		BlackboardComponent->SetValueAsBool(InAttackRangeKey.SelectedKeyName, false);
		return;
	}

	const bool bHasLineOfSight = AIController->LineOfSightTo(TargetActor);
	BlackboardComponent->SetValueAsBool(InAttackRangeKey.SelectedKeyName, bHasLineOfSight);
}
