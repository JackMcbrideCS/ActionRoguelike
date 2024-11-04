// Fill out your copyright notice in the Description page of Project Settings.


#include "ARMagicProjectile.h"

#include "ARAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AARMagicProjectile::AARMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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

	AttributeComponent->ApplyHealthChange(GetInstigator(), Damage * -1.0f);
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AARMagicProjectile::Explode_Implementation()
{
	Super::Explode_Implementation();
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), ExplodeCameraShake, GetActorLocation(), 0.0f, 1000.0f);
}

