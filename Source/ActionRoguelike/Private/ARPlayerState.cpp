// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPlayerState.h"

bool AARPlayerState::HasEnoughCredits(int32 Amount)
{
	return Credits >= Amount;
}

void AARPlayerState::GainCredits(int32 Amount)
{
	Credits += Amount;
	OnCreditsChanged.Broadcast(this, Credits, Amount);
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
	OnCreditsChanged.Broadcast(this, Credits, -Amount);
	return true;
}
