// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ARPlayerState.generated.h"

class UARSaveGame;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AARPlayerState*, OwningPlayerState, int32, NewAmount,
                                               int32, Delta);

UCLASS()
class ACTIONROGUELIKE_API AARPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;
	
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastCreditsChanged(int32 NewAmount, int32 Delta);

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(UARSaveGame* SaveGame);
	
	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(UARSaveGame* SaveGame);
	
	bool HasEnoughCredits(int32 Amount);
	void GainCredits(int32 Amount);
	bool SpendCredits(int32 Amount);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UPROPERTY(BlueprintReadOnly, Replicated)
	int32 Credits;
};
