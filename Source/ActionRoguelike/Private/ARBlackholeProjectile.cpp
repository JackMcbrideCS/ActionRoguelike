// Fill out your copyright notice in the Description page of Project Settings.


#include "ARBlackholeProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AARBlackholeProjectile::AARBlackholeProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ForceComponent = CreateDefaultSubobject<URadialForceComponent>("Force Component");
	ForceComponent->SetupAttachment(SphereComponent);
}

// Called when the game starts or when spawned
void AARBlackholeProjectile::BeginPlay()
{
	Super::BeginPlay();

	ParticleComponent->OnSystemFinished.AddDynamic(this, &AARBlackholeProjectile::OnParticlesFinished);
}

void AARBlackholeProjectile::OnParticlesFinished(UParticleSystemComponent* PSystem)
{
	Destroy();
}

void AARBlackholeProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bool bIgnore = !OtherComp->IsSimulatingPhysics() || OtherComp->GetCollisionObjectType() != ECC_Destructible;
	if (bIgnore)
	{
		return;
	}
	
	OtherActor->Destroy();
}

void AARBlackholeProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	MovementComponent->StopMovementImmediately();
}
