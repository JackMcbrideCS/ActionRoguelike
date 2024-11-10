// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "ARItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARItemChest : public AActor, public IARGameplayInterface
{
	GENERATED_BODY()

	virtual bool CanInteract_Implementation(APawn* InstigatorPawn) const override;
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	virtual void OnActorLoaded_Implementation() override;

public:
	UPROPERTY(EditAnywhere)
	float TargetPitch;
	
	// Sets default values for this actor's properties
	AARItemChest();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	UPROPERTY(ReplicatedUsing = "OnRep_LidOpened", Replicated, SaveGame)
	bool bLidOpen;

	UFUNCTION()
	void OnRep_LidOpened();
};
