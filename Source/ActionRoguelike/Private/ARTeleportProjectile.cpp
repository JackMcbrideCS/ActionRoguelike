// Fill out your copyright notice in the Description page of Project Settings.


#include "ARTeleportProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AARTeleportProjectile::AARTeleportProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AARTeleportProjectile::TravelTime_TimerElapsed()
{
	Explode();
}

void AARTeleportProjectile::Teleport()
{
	if (!ensure(GetInstigator()))
	{
		return;
	}
	
	GetInstigator()->TeleportTo(GetActorLocation(), GetActorRotation());
	Destroy();
}

void AARTeleportProjectile::Explode_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetTransform());
	GetWorldTimerManager().ClearTimer(TimerHandle_TravelTime);
	MovementComponent->StopMovementImmediately();
	ParticleComponent->DeactivateSystem();
	SetActorEnableCollision(false);
	if (TeleportDelay <= 0.0f)
	{
		Teleport();
	}
	else
	{
		GetWorldTimerManager().SetTimer(TimerHandle_TeleportDelay, this, &AARTeleportProjectile::Teleport, TeleportDelay);
	}
}
