// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "ARPickUpBase.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARPickUpBase : public AActor, public IARGameplayInterface
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RespawnTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	// Sets default values for this actor's properties
	AARPickUpBase();
	
	virtual bool CanInteract_Implementation(APawn* InstigatorPawn) const override;
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:

	FTimerHandle TimerHandle_Respawn;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ApplyEffect(APawn* Pawn);

	void Respawn();
};
