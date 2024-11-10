// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/ARActionComponent.h"

#include "ActionRoguelike/ActionRoguelike.h"
#include "Actions/ARAction.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

UARActionComponent::UARActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
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

		if (!GetOwner()->HasAuthority())
		{
			ServerStartAction(Instigator, ActionName);
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

void UARActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void UARActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (const TSubclassOf<UARAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void UARActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//const FString DebugMessage = FString::Printf(TEXT("%s : %s"), *GetNameSafe(GetOwner()), *ActiveGameplayTags.ToStringSimple());
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMessage);

	for (UARAction* Action : Actions)
	{
		FColor TextColour = Action->IsRunning() ? FColor::Blue : FColor::White;
		FString ActionMessage = FString::Printf(TEXT("[%s] Action: %s : IsRunning: %s : Outer: %s"),
			*GetNameSafe(GetOwner()),
			*Action->ActionName.ToString(),
			Action->IsRunning() ? TEXT("true") : TEXT("false"),
			*GetNameSafe(GetOuter()));

		LogOnScreen(this, ActionMessage, TextColour, 0.0f);
	}
}

void UARActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARActionComponent, Actions);
}

bool UARActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UARAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

