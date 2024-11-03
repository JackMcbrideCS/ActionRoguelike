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

bool UARAttributeComponent::ApplyHealthChange(float Delta)
{
	const float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	Delta = Health - OldHealth;
	if (Delta == 0.0f)
	{
		return false;
	}
	
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
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

