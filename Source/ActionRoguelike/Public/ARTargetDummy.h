// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARTargetDummy.generated.h"

class UARAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API AARTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AARTargetDummy();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UARAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UStaticMeshComponent* MeshComponent;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComponent, float NewHealth, float Delta);
};
