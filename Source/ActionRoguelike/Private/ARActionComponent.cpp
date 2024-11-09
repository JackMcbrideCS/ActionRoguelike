// Fill out your copyright notice in the Description page of Project Settings.


#include "ARActionComponent.h"

#include "ARAction.h"

UARActionComponent::UARActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UARActionComponent::AddAction(AActor* Instigator, TSubclassOf<UARAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UARAction* NewAction = NewObject<UARAction>(this, ActionClass);
	if (!ensure(NewAction))
	{
		return;
	}

	Actions.Add(NewAction);
	if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
	{
		NewAction->StartAction(Instigator);
	}
}

void UARActionComponent::RemoveAction(UARAction* Action)
{
	bool bIsInactive = Action && !Action->IsRunning();
	if (!ensureAlways(bIsInactive))
	{
		return;
	}

	Actions.Remove(Action);
}

bool UARActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UARAction* Action : Actions)
	{
		if (!Action || Action->ActionName != ActionName)
		{
			continue;
		}

		if (!Action->CanStart(Instigator))
		{
			FString FailedMessage = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMessage);
			continue;
		}

		Action->StartAction(Instigator);
		return true;
	}

	return false;
}

bool UARActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UARAction* Action : Actions)
	{
		if (!Action->IsRunning())
		{
			continue;
		}
		
		if (!Action || Action->ActionName != ActionName)
		{
			continue;
		}

		Action->StopAction(Instigator);
		return true;
	}

	return false;
}

void UARActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const TSubclassOf<UARAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}

void UARActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FString DebugMessage = FString::Printf(TEXT("%s : %s"), *GetNameSafe(GetOwner()), *ActiveGameplayTags.ToStringSimple());
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMessage);
}

