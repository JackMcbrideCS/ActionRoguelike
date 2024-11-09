// Fill out your copyright notice in the Description page of Project Settings.


#include "ARInteractionComponent.h"

#include "ARCharacter.h"
#include "ARGameplayInterface.h"
#include "KismetTraceUtils.h"
#include "Blueprint/UserWidget.h"
#include "UI/ARWorldUserWidget.h"

UARInteractionComponent::UARInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UARInteractionComponent::PrimaryInteract()
{
	if (!FocusedActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to Interact");
		return;
	}
	
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if(!IARGameplayInterface::Execute_CanInteract(FocusedActor, MyPawn))
	{
		return;
	}
		
	IARGameplayInterface::Execute_Interact(FocusedActor, MyPawn);
}

void UARInteractionComponent::FindBestInteractable()
{
	AARCharacter* OwnerCharacter = Cast<AARCharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		return;
	}
	
	TArray<FHitResult> Hits;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(TraceRadius);
	OwnerCharacter->AimSweep(Hits, TraceLength, ObjectQueryParams, CollisionShape);

	FocusedActor = nullptr;
	for (const FHitResult& Hit: Hits)
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

		FocusedActor = HitActor;
		break;
	}

	if (!FocusedActor)
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
		return;
	}

	if (!DefaultWidgetInstance && ensure(DefaultWidgetClass))
	{
		DefaultWidgetInstance = CreateWidget<UARWorldUserWidget>(GetWorld(), DefaultWidgetClass);
	}

	if (!DefaultWidgetInstance)
	{
		return;
	}

	DefaultWidgetInstance->AttachedActor = FocusedActor;
	if (DefaultWidgetInstance->IsInViewport())
	{
		return;
	}
	
	DefaultWidgetInstance->AddToViewport();
}

void UARInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteractable();
}

void UARInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

