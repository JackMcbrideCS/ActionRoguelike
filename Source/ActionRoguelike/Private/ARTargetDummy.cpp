// Fill out your copyright notice in the Description page of Project Settings.


#include "ARTargetDummy.h"

#include "ARAttributeComponent.h"

// Sets default values
AARTargetDummy::AARTargetDummy()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Component");
    RootComponent = MeshComponent;
    
	AttributeComponent = CreateDefaultSubobject<UARAttributeComponent>("Attribute Component");
	AttributeComponent->OnHealthChanged.AddDynamic(this, &AARTargetDummy::OnHealthChanged);
}

void AARTargetDummy::OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComponent, float NewHealth,
	float Delta)
{
	if (NewHealth >= 0)
	{
		return;
	}
	
	MeshComponent->SetScalarParameterValueOnMaterials(TEXT("HitTime"), GetWorld()->GetTimeSeconds());
}

