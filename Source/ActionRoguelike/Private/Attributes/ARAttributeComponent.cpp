// Fill out your copyright notice in the Description page of Project Settings.


#include "Attributes/ARAttributeComponent.h"

#include "ARGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("ar.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Components."), ECVF_Cheat);

// Sets default values for this component's properties
UARAttributeComponent::UARAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 100.0f;
	Health = MaxHealth;
	Rage = 0.0f;
}

void UARAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	Rage = 0.0f;
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

	if (Delta < 0.0f)
	{
		Delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}
	
	const float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	Delta = Health - OldHealth;
	if (Delta == 0.0f)
	{
		return false;
	}

	if (Health == 0.0f)
	{
		AARGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AARGameModeBase>();
		if (ensure(GameMode))
		{
			GameMode->OnActorKilled(Instigator, GetOwner());
		}
	}
	
	OnHealthChanged.Broadcast(Instigator, this, Health, Delta);
	return true;
}

bool UARAttributeComponent::ApplyRageChange(AActor* Instigator, float Delta)
{
	const float OldRage = Rage;
	Rage = FMath::Clamp(Rage + Delta, 0.0f, MaxRage);
	Delta = Rage - OldRage;
	if (Delta == 0.0f)
	{
		return false;
	}
	
	OnRageChanged.Broadcast(Instigator, this, Rage, Delta);
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

float UARAttributeComponent::GetMaxRage() const
{
	return MaxRage;
}

float UARAttributeComponent::GetRage() const
{
	return Rage;
}

bool UARAttributeComponent::Kill(AActor* Instigator)
{
	return ApplyHealthChange(Instigator, -MaxHealth);
}

