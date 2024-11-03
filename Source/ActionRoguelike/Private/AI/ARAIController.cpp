// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AARAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviourTree);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (ensure(PlayerPawn))
	{
		GetBlackboardComponent()->SetValueAsVector(TEXT("MoveToLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), PlayerPawn);
	}
}
