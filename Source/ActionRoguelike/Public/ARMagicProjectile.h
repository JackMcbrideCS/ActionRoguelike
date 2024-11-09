// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARProjectileBase.h"
#include "GameplayTagContainer.h"
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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> ExplodeCameraShake;

	UPROPERTY(EditDefaultsOnly)
	float Damage;
	
	UPROPERTY(EditDefaultsOnly)
	float DamageImpulseStrength;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ParryTag;
	
	virtual void TravelTime_TimerElapsed() override;
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	
	virtual void Explode_Implementation() override;
};
