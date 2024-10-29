// Fill out your copyright notice in the Description page of Project Settings.


#include "ARMagicProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AARMagicProjectile::AARMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere Component");
	SphereComponent->SetCollisionProfileName("Projectile");
	RootComponent = SphereComponent;

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>("Particle Component");
	ParticleComponent->SetupAttachment(SphereComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Movement Component");
	MovementComponent->InitialSpeed = 1000.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;
}

// Called when the game starts or when spawned
void AARMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (GetInstigator())
	{
		SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
		GetInstigator()->MoveIgnoreActorAdd(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile %s does not have an Instigator."), *GetNameSafe(this));
	}
}

// Called every frame
void AARMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

