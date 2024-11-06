// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAttributeComponent.h"

// Sets default values for this component's properties
UARAttributeComponent::UARAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 100.0f;
	Health = MaxHealth;
}

void UARAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}

UARAttributeComponent* UARAttributeComponent::GetAttributes(const AActor* Actor)
{
	if (!Actor)
	{
		return nullptr;
	}

	return Actor->GetComponentByClass<UARAttributeComponent>();
}

bool UARAttributeComponent::IsActorAlive(const AActor* Actor)
{
	const UARAttributeComponent* AttributeComponent = GetAttributes(Actor);
	if (!AttributeComponent)
	{
		return false;
	}

	return AttributeComponent->IsAlive();
}

bool UARAttributeComponent::KillActor(AActor* Instigator, AActor* Actor)
{
	UARAttributeComponent* AttributeComponent = GetAttributes(Actor);
	if (!AttributeComponent)
	{
		return false;
	}

	return AttributeComponent->Kill(Instigator);
}

bool UARAttributeComponent::ApplyHealthChange(AActor* Instigator, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}
	
	const float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	Delta = Health - OldHealth;
	if (Delta == 0.0f)
	{
		return false;
	}
	
	OnHealthChanged.Broadcast(Instigator, this, Health, Delta);
	return true;
}

bool UARAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

float UARAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UARAttributeComponent::GetHealth() const
{
	return Health;
}

bool UARAttributeComponent::Kill(AActor* Instigator)
{
	return ApplyHealthChange(Instigator, -MaxHealth);
}

