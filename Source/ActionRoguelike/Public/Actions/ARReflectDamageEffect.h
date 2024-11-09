// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/ARActionEffect.h"
#include "ARReflectDamageEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARReflectDamageEffect : public UARActionEffect
{
	GENERATED_BODY()

public:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComponent, float NewValue, float Delta);
};
