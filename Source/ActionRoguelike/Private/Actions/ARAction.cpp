// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/ARAction.h"

#include "ActionRoguelike/ActionRoguelike.h"
#include "Actions/ARActionComponent.h"
#include "Attributes/ARAttributeComponent.h"
#include "Net/UnrealNetwork.h"

void UARAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Running: %s"), *ActionName.ToString()), FColor::Blue);

	UARActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);
	bIsRunning = true;
}

void UARAction::StopAction_Implementation(AActor* Instigator)
{
	UARActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);
	bIsRunning = false;

	LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);
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
	const UActorComponent* Component = GetOwningComponent();
	if (!ensure(Component))
	{
		return nullptr;
	}
	
	return Component->GetWorld();
}

void UARAction::OnRep_IsRunning()
{
	if (bIsRunning)
	{
		StartAction(nullptr);
	}
	else
	{
		StopAction(nullptr);
	}
}

UARActionComponent* UARAction::GetOwningComponent() const
{
	return Cast<UARActionComponent>(GetOuter());
}

UARAttributeComponent* UARAction::GetOwnerAttributes() const
{
	return UARAttributeComponent::GetAttributes(GetOwner());
}

AActor* UARAction::GetOwner() const
{
	return GetOwningComponent()->GetOwner();
}

void UARAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARAction, bIsRunning);
}
