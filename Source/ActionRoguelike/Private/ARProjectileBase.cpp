// Fill out your copyright notice in the Description page of Project Settings.


#include "ARProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AARProjectileBase::AARProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere Component");
	SphereComponent->SetCollisionProfileName("Projectile");
	RootComponent = SphereComponent;
	
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Movement Component");
	MovementComponent->InitialSpeed = 1000.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;
}

// Called when the game starts or when spawned
void AARProjectileBase::BeginPlay()
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
void AARProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
