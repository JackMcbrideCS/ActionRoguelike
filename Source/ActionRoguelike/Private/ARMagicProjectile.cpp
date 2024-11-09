// Fill out your copyright notice in the Description page of Project Settings.


#include "ARMagicProjectile.h"

#include "ARActionEffect.h"
#include "ARActionComponent.h"
#include "ARAttributeComponent.h"
#include "ARGameplayFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AARMagicProjectile::AARMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 40.0f;
	DamageImpulseStrength = 150000.0f;
}

void AARMagicProjectile::TravelTime_TimerElapsed()
{
	Super::TravelTime_TimerElapsed();
	Destroy();
}

void AARMagicProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
	{
		return;
	}

	UARActionComponent* ActionComponent = OtherActor->GetComponentByClass<UARActionComponent>();
	if (ActionComponent && ActionComponent->ActiveGameplayTags.HasTag(ParryTag))
	{
		MovementComponent->Velocity = -MovementComponent->Velocity;
		SetInstigator(Cast<APawn>(OtherActor));
		return;
	}

	if (!UARAttributeComponent::GetAttributes(OtherActor))
	{
		return;
	}

	UARGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, DamageImpulseStrength, SweepResult);
	if (ActionComponent)
	{
		for (TSubclassOf<UARActionEffect> AppliedEffectClass : AppliedEffectClasses)
		{
			ActionComponent->AddAction(GetInstigator(), AppliedEffectClass);
		}
	}
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AARMagicProjectile::Explode_Implementation()
{
	Super::Explode_Implementation();
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), ExplodeCameraShake, GetActorLocation(), 0.0f, 1000.0f);
}

