// Fill out your copyright notice in the Description page of Project Settings.


#include "ARHealthPickup.h"

#include "ARAttributeComponent.h"

bool AARHealthPickup::CanInteract_Implementation(APawn* InstigatorPawn) const
{
	Super::CanInteract_Implementation(InstigatorPawn);
	const UARAttributeComponent* AttributeComponent = InstigatorPawn->GetComponentByClass<UARAttributeComponent>();
	if (!AttributeComponent)
	{
		return false;
	}

	return AttributeComponent->GetHealth() < AttributeComponent->GetMaxHealth();
}

void AARHealthPickup::ApplyEffect_Implementation(APawn* Pawn)
{
	Super::ApplyEffect_Implementation(Pawn);
	UARAttributeComponent* AttributeComponent = UARAttributeComponent::GetAttributes(Pawn);
	if (!AttributeComponent)
	{
		return;
	}

	AttributeComponent->ApplyHealthChange(this, 20.0f);
}
