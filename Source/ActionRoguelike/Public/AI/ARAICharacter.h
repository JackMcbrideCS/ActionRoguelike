// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARAICharacter.generated.h"

class UARWorldUserWidget;
class UWidget;
class UARAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class ACTIONROGUELIKE_API AARAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARAICharacter();

protected:

	UPROPERTY(VisibleAnywhere, Category = Components)
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, Category = Components)
	UARAttributeComponent* AttributeComponent;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	int32 CreditGainOnKill;

	UARWorldUserWidget* HealthBarWidget;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComponent, float NewHealth, float Delta);
	
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	void SetTargetActor(AActor* NewTarget) const;

	virtual void BeginPlay() override;
};
