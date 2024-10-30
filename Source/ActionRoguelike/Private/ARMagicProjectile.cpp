// Fill out your copyright notice in the Description page of Project Settings.


#include "ARMagicProjectile.h"

#include "ARAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AARMagicProjectile::AARMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>("Particle Component");
	ParticleComponent->SetupAttachment(SphereComponent);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AARMagicProjectile::OnBeginOverlap);
}

// Called when the game starts or when spawned
void AARMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
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

	UActorComponent* Component = OtherActor->GetComponentByClass(UARAttributeComponent::StaticClass());
	UARAttributeComponent* AttributeComponent = Cast<UARAttributeComponent>(Component);
	if (!AttributeComponent)
	{
		return;
	}

	AttributeComponent->ApplyHealthChange(-20.0f);
	Destroy();
}

// Called every frame
void AARMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

