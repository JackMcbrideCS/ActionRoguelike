// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API AARProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere)
	float Lifetime;
	
	// Sets default values for this actor's properties
	AARProjectileBase();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComponent;

	FTimerHandle TimerHandle_Lifetime;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Lifetime_TimerElapsed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
