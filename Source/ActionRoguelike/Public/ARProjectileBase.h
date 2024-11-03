// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystem;

UCLASS()
class ACTIONROGUELIKE_API AARProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere)
	float TravelTime;
	
	// Sets default values for this actor's properties
	AARProjectileBase();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UParticleSystem* HitParticle;

	FTimerHandle TimerHandle_TravelTime;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void TravelTime_TimerElapsed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
