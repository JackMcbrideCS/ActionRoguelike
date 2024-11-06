// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ARPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AARPlayerState*, OwningPlayerState, int32, NewAmount, int32, Delta);

UCLASS()
class ACTIONROGUELIKE_API AARPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;
	
	bool HasEnoughCredits(int32 Amount);
	void GainCredits(int32 Amount);
	bool SpendCredits(int32 Amount);

protected:

	int32 Credits;
};
