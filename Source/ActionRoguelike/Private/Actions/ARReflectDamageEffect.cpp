// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/ARReflectDamageEffect.h"

#include "Attributes/ARAttributeComponent.h"

void UARReflectDamageEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	GetOwnerAttributes()->OnHealthChanged.AddDynamic(this, &UARReflectDamageEffect::OnHealthChanged);
}

void UARReflectDamageEffect::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
	GetOwnerAttributes()->OnHealthChanged.RemoveDynamic(this, &UARReflectDamageEffect::OnHealthChanged);
}

void UARReflectDamageEffect::OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComponent,
	float NewValue, float Delta)
{
	if (Delta >= 0.0f)
	{
		return;
	}

	AActor* Owner = GetOwner();
	if (InstigatorActor == Owner)
	{
		return;
	}
	
	UARAttributeComponent* InstigatorAttributeComponent = UARAttributeComponent::GetAttributes(InstigatorActor);
	if (!InstigatorAttributeComponent)
	{
		return;
	}

	InstigatorAttributeComponent->ApplyHealthChange(Owner, Delta);
}
