// Fill out your copyright notice in the Description page of Project Settings.


#include "ARInteractionComponent.h"

#include "ARCharacter.h"
#include "ARGameplayInterface.h"
#include "KismetTraceUtils.h"

void UARInteractionComponent::PrimaryInteract()
{
	AARCharacter* OwnerCharacter = Cast<AARCharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		return;
	}
	
	TArray<FHitResult> Hits;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	FCollisionShape CollisionShape;
	const float Radius = 30.0f;
	CollisionShape.SetSphere(Radius);
	OwnerCharacter->AimSweep(Hits, 1000.0f, ObjectQueryParams, CollisionShape);
	
	for(const FHitResult& Hit: Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor)
		{
			continue;
		}

		if (!HitActor->Implements<UARGameplayInterface>())
		{
			continue;
		}

		APawn* MyPawn = Cast<APawn>(GetOwner());
		if(!IARGameplayInterface::Execute_CanInteract(HitActor, MyPawn))
		{
			return;
		}
		
		IARGameplayInterface::Execute_Interact(HitActor, MyPawn);
		return;
	}
}

