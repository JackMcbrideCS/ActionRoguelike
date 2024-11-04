// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARBTTask_RangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UARBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	const ACharacter* AICharacter = Cast<ACharacter>(AIController->GetPawn());
	if (!ensure(AICharacter))
	{
		return EBTNodeResult::Failed;
	}

	if (!ensure(AICharacter->GetMesh()->DoesSocketExist(TEXT("Muzzle_01"))))
	{
		return EBTNodeResult::Failed;
	}
	const FVector MuzzleLocation = AICharacter->GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));

	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!ensure(BlackboardComponent))
	{
		return EBTNodeResult::Failed;
	}

	const AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetActorRangeKey.SelectedKeyName));
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	const FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
	FRotator MuzzleRotation = Direction.Rotation();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = AIPawn;
	
	AActor* SpawnedProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParameters);
	return SpawnedProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
