// Fill out your copyright notice in the Description page of Project Settings.


#include "ARInteractionComponent.h"

#include "ARGameplayInterface.h"
#include "KismetTraceUtils.h"

void UARInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	FVector EyeLocation;
	FRotator EyeRotation;
	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	const FVector End = EyeLocation + EyeRotation.Vector() * 1000.0f;

	TArray<FHitResult> Hits;
	FCollisionShape CollisionShape;
	const float Radius = 30.0f;
	CollisionShape.SetSphere(Radius);

	const bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, CollisionShape);
	DrawDebugSweptSphere(GetWorld(), EyeLocation, End, Radius, bBlockingHit ? FColor::Green : FColor::Red, false, 2.0f, 0);
	
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

