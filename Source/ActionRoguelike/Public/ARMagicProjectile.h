// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARProjectileBase.h"
#include "GameFramework/Actor.h"
#include "ARMagicProjectile.generated.h"

class UParticleSystemComponent;

UCLASS()
class ACTIONROGUELIKE_API AARMagicProjectile : public AARProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARMagicProjectile();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* ParticleComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void TravelTime_TimerElapsed() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
