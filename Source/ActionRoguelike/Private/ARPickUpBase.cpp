// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPickUpBase.h"

#include "ARAttributeComponent.h"
#include "ARCharacter.h"

void AARPickUpBase::Interact_Implementation(APawn* InstigatorPawn)
{
	MeshComponent->SetVisibility(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ApplyEffect(InstigatorPawn);
	GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &AARPickUpBase::Respawn, RespawnTime);
}

// Sets default values
AARPickUpBase::AARPickUpBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Component");
	RootComponent = MeshComponent;
}

bool AARPickUpBase::CanInteract_Implementation(APawn* InstigatorPawn) const
{
	return true;
}

void AARPickUpBase::Respawn()
{
	MeshComponent->SetVisibility(true);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AARPickUpBase::ApplyEffect_Implementation(APawn* Pawn)
{
}

