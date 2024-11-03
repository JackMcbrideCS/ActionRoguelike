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

public:
	UPROPERTY(EditAnywhere)
	float TargetPitch;
	
	// Sets default values for this actor's properties
	AARItemChest();

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
