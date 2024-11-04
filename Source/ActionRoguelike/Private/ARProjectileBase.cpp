// Fill out your copyright notice in the Description page of Project Settings.


#include "ARProjectileBase.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AARProjectileBase::AARProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere Component");
	SphereComponent->SetCollisionProfileName("Projectile");
	RootComponent = SphereComponent;

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>("Particle Component");
	ParticleComponent->SetupAttachment(RootComponent);
	
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio Component");
	AudioComponent->SetupAttachment(RootComponent);
	
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

	if (TravelTime > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_TravelTime, this, &AARProjectileBase::TravelTime_TimerElapsed, TravelTime);
	}

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AARProjectileBase::OnBeginOverlap);
	SphereComponent->OnComponentHit.AddDynamic(this, &AARProjectileBase::OnHit);
}

void AARProjectileBase::TravelTime_TimerElapsed()
{
	MovementComponent->StopMovementImmediately();
}

void AARProjectileBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Explode();
}

void AARProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void AARProjectileBase::Explode_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetTransform());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSoundCue, GetActorLocation());
	Destroy();
}

