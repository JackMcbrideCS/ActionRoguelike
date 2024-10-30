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

	UPROPERTY(EditAnywhere)
	UParticleSystem* ExplosionParticle;
	
	// Sets default values for this actor's properties
	AARTeleportProjectile();
	UFUNCTION()
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* ParticleComponent;

	FTimerHandle TimerHandle_TeleportDelay;

	virtual void TravelTime_TimerElapsed() override;
	void TravelEnded();
	void Teleport();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
