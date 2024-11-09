// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARAction.h"
#include "ARActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARActionEffect : public UARAction
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	float Duration;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	float Period;

	FTimerHandle TimerHandle_Duration;
	FTimerHandle TimerHandle_Period;

	UFUNCTION(BlueprintNativeEvent, Category = Effect)
	void ExecutePeriodicEffect(AActor* Instigator);

public:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

	UARActionEffect();
};
