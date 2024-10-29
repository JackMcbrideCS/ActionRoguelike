// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARProjectileBase.h"
#include "ARBlackholeProjectile.generated.h"

class URadialForceComponent;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARBlackholeProjectile : public AARProjectileBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AARBlackholeProjectile();
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* ParticleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URadialForceComponent* ForceComponent;

	UFUNCTION()
	void OnParticlesFinished(UParticleSystemComponent* PSystem);
};
