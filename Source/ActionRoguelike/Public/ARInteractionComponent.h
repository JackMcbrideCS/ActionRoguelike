// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARInteractionComponent.generated.h"


class UARWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UARInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UARInteractionComponent();
	
	void PrimaryInteract();

protected:

	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UARWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	UARWorldUserWidget* DefaultWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = Trace)
	float TraceLength = 1000.0f;

	UPROPERTY(EditDefaultsOnly, Category = Trace)
	float TraceRadius = 30.0f;

	UPROPERTY(EditDefaultsOnly, Category = Trace)
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_WorldDynamic;

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* Target);
	
	void FindBestInteractable();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
};
