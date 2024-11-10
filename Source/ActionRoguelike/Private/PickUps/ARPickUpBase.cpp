// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUps/ARPickUpBase.h"

#include "Attributes/ARAttributeComponent.h"
#include "ARCharacter.h"
#include "ARPlayerState.h"
#include "Net/UnrealNetwork.h"

void AARPickUpBase::Interact_Implementation(APawn* InstigatorPawn)
{
	SetActive(false);
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

	bReplicates = true;
}

bool AARPickUpBase::CanInteract_Implementation(APawn* InstigatorPawn) const
{
	AARPlayerState* InstigatorPlayerState = InstigatorPawn->GetController()->GetPlayerState<AARPlayerState>();
	if (ensure(InstigatorPlayerState) && !InstigatorPlayerState->HasEnoughCredits(PickUpCreditCost))
	{
		return false;	
	}

	return true;
}

void AARPickUpBase::OnRep_IsActive()
{
	RootComponent->SetVisibility(bIsActive);
	SetActorEnableCollision(bIsActive);
}

void AARPickUpBase::Respawn()
{
	SetActive(true);
}

void AARPickUpBase::SetActive(bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	OnRep_IsActive();
}

void AARPickUpBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARPickUpBase, bIsActive);
}

void AARPickUpBase::ApplyEffect_Implementation(APawn* Pawn)
{
	AARPlayerState* PlayerState = Pawn->GetController()->GetPlayerState<AARPlayerState>();
	if(!ensure(PlayerState))
	{
		return;
	}
	
	PlayerState->SpendCredits(PickUpCreditCost);
	PlayerState->GainCredits(PickUpCreditGain);
}

