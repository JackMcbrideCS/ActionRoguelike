// Fill out your copyright notice in the Description page of Project Settings.


#include "ARExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
AARExplosiveBarrel::AARExplosiveBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh Component");
	StaticMeshComponent->SetSimulatePhysics(true);
	RootComponent = StaticMeshComponent;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("Radial Force Component");
	RadialForceComponent->SetupAttachment(StaticMeshComponent);
	RadialForceComponent->SetAutoActivate(false);
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->ImpulseStrength = 2500.0f;
	RadialForceComponent->Radius = 750.0f;
	RadialForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

// Called when the game starts or when spawned
void AARExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &AARExplosiveBarrel::OnStaticMeshHit);
}

void AARExplosiveBarrel::OnStaticMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComponent->FireImpulse();
}

// Called every frame
void AARExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

