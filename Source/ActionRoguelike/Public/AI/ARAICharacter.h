// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARAICharacter.generated.h"

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

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
};
