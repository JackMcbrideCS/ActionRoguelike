// Fill out your copyright notice in the Description page of Project Settings.


#include "ARTeleportProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

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
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSoundCue, GetActorLocation());
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
