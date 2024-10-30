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
		IARGameplayInterface::Execute_Interact(HitActor, MyPawn);
		return;
	}
}

// Sets default values for this component's properties
UARInteractionComponent::UARInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UARInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UARInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

