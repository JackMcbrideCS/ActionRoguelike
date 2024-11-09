// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/ARAction_ProjectileAttack.h"

#include "ARCharacter.h"
#include "Projectiles/ARProjectileBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UARAction_ProjectileAttack::UARAction_ProjectileAttack()
{
	AttackDelay = 0.2f;
	MuzzleSocketName = "Muzzle_01";
}

void UARAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (!Character)
	{
		return;
	}

	Character->PlayAnimMontage(AnimMontage);
	UGameplayStatics::SpawnEmitterAttached(CastingParticleEffect, Character->GetMesh(), MuzzleSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

	FTimerDelegate Delegate;
	FTimerHandle TimerHandle_AttackDelay;
	Delegate.BindUFunction(this, TEXT("AttackDelay_TimerElapsed"), Character);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackDelay, false);
}

void UARAction_ProjectileAttack::AttackDelay_TimerElapsed(ACharacter* InstigatorCharacter)
{
	AARCharacter* Character = Cast<AARCharacter>(InstigatorCharacter);
	if (!ensure(Character))
	{
		StopAction(Character);
		return;
	}
	
	const bool bSocketExists = Character->GetMesh()->DoesSocketExist(MuzzleSocketName);
	if (!ensure(bSocketExists))
	{
		StopAction(Character);
		return;
	}
	
	const FVector SpawnLocation = Character->GetMesh()->GetSocketLocation(MuzzleSocketName);
	
	FHitResult Hit;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	const bool bBlockingHit = Character->AimTrace(Hit, 1000.0f, ObjectQueryParams);
	
	const FVector LookAtPosition = bBlockingHit ? Hit.ImpactPoint : Hit.TraceEnd;
	const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, LookAtPosition);
	const FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = Character;
	GetWorld()->SpawnActor<AARProjectileBase>(ProjectileClass, SpawnTransform, SpawnParameters);
	StopAction(Character);
}
