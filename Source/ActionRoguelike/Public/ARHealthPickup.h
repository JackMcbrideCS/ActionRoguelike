// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPickUpBase.h"
#include "ARHealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARHealthPickup : public AARPickUpBase
{
	GENERATED_BODY()

public:

	virtual bool CanInteract_Implementation(APawn* InstigatorPawn) const override;
	virtual void ApplyEffect_Implementation(APawn* Pawn) override;
};
