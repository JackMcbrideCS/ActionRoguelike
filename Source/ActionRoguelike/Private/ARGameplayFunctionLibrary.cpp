// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGameplayFunctionLibrary.h"

#include "Attributes/ARAttributeComponent.h"

bool UARGameplayFunctionLibrary::ApplyDamage(AActor* Instigator, AActor* TargetActor, float DamageAmount)
{
	if (Instigator == TargetActor)
	{
		return false;
	}
	
	UARAttributeComponent* AttributeComponent = UARAttributeComponent::GetAttributes(TargetActor);
	if (!AttributeComponent)
	{
		return false;
	}

	return AttributeComponent->ApplyHealthChange(Instigator, -DamageAmount);
}

bool UARGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* Instigator, AActor* TargetActor, float DamageAmount,
	float ImpulseStrength, const FHitResult& Hit)
{
	if (!ApplyDamage(Instigator, TargetActor, DamageAmount))
	{
		return false;
	}

	UPrimitiveComponent* HitComponent = Hit.GetComponent();
	if (HitComponent && HitComponent->IsSimulatingPhysics(Hit.BoneName))
	{
		FVector Direction = Hit.TraceEnd - Hit.TraceStart;
		Direction.Normalize();
		HitComponent->AddImpulseAtLocation(Direction * ImpulseStrength, Hit.ImpactPoint, Hit.BoneName);
	}

	return true;
}
