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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 PickUpCreditCost;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 PickUpCreditGain;

	UPROPERTY(ReplicatedUsing = "OnRep_IsActive")
	bool bIsActive = true;
	
	FTimerHandle TimerHandle_Respawn;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ApplyEffect(APawn* Pawn);
	
	UFUNCTION()
	void OnRep_IsActive();
	
	virtual void Respawn();
	void SetActive(bool bNewIsActive);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
