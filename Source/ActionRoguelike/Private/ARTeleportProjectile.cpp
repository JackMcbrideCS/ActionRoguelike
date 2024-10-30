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
	
	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>("Particle Component");
	ParticleComponent->SetupAttachment(SphereComponent);
}

// Called when the game starts or when spawned
void AARTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentHit.AddDynamic(this, &AARTeleportProjectile::OnHit);
}

void AARTeleportProjectile::Lifetime_TimerElapsed()
{
	Super::Lifetime_TimerElapsed();
	TravelEnded();
}

void AARTeleportProjectile::TravelEnded()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Lifetime);
	MovementComponent->StopMovementImmediately();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetTransform());
	ParticleComponent->Deactivate();
	if (TeleportDelay <= 0.0f)
	{
		Teleport();
	}
	else
	{
		GetWorldTimerManager().SetTimer(TimerHandle_TeleportDelay, this, &AARTeleportProjectile::Teleport, TeleportDelay);
	}
}

void AARTeleportProjectile::Teleport()
{
	if (!GetInstigator())
	{
		return;
	}
	
	GetInstigator()->TeleportTo(GetActorLocation(), GetActorRotation());
	Destroy();
}

void AARTeleportProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	TravelEnded();
}
