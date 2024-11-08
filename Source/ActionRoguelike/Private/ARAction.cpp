// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAction.h"

#include "ARActionComponent.h"

void UARAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	UARActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);
	bIsRunning = true;
}

void UARAction::StopAction_Implementation(AActor* Instigator)
{
	ensureAlways(bIsRunning);
	UARActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);
	bIsRunning = false;

	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
}

bool UARAction::CanStart_Implementation(AActor* Instigator) const
{
	if (bIsRunning)
	{
		return false;
	}
	
	UARActionComponent* ActionComponent = GetOwningComponent();
	if (ActionComponent->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

bool UARAction::IsRunning_Implementation() const
{
	return bIsRunning;
}

UWorld* UARAction::GetWorld() const
{
	const UActorComponent* Component = Cast<UActorComponent>(GetOuter());
	if (!ensure(Component))
	{
		return nullptr;
	}
	
	return Component->GetWorld();
}

UARActionComponent* UARAction::GetOwningComponent() const
{
	return Cast<UARActionComponent>(GetOuter());
}
