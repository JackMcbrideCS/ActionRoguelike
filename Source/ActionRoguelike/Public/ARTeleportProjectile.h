// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARProjectileBase.h"
#include "ARTeleportProjectile.generated.h"

class UParticleSystem;

UCLASS()
class ACTIONROGUELIKE_API AARTeleportProjectile : public AARProjectileBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float TeleportDelay;

protected:
	FTimerHandle TimerHandle_TeleportDelay;

	virtual void TravelTime_TimerElapsed() override;
	void Teleport();
	virtual void Explode_Implementation() override;
};
