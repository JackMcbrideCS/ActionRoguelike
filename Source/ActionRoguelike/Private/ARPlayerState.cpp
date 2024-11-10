// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPlayerState.h"

#include "Net/UnrealNetwork.h"

void AARPlayerState::MultiCastCreditsChanged_Implementation(int32 NewAmount, int32 Delta)
{
	OnCreditsChanged.Broadcast(this, NewAmount, Delta);
}

bool AARPlayerState::HasEnoughCredits(int32 Amount)
{
	return Credits >= Amount;
}

void AARPlayerState::GainCredits(int32 Amount)
{
	if (Amount == 0)
	{
		return;
	}
	
	Credits += Amount;
	MultiCastCreditsChanged(Credits, Amount);
}

bool AARPlayerState::SpendCredits(int32 Amount)
{
	if (Amount == 0)
	{
		return true;
	}
	
	if (!HasEnoughCredits(Amount))
	{
		return false;
	}

	Credits -= Amount;
	MultiCastCreditsChanged(Credits, -Amount);
	return true;
}

void AARPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AARPlayerState, Credits);
}
