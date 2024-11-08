// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAction.h"

#include "ARActionComponent.h"

void UARAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	UARActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);
}

void UARAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	
	UARActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);
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
