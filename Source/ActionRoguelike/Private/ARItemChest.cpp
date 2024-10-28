// Fill out your copyright notice in the Description page of Project Settings.


#include "ARItemChest.h"


void AARItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0, 0));
}

// Sets default values
AARItemChest::AARItemChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("Lid Mesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.0f;
}

// Called when the game starts or when spawned
void AARItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

