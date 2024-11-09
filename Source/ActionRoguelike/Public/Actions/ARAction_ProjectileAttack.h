// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARAction.h"
#include "ARAction_ProjectileAttack.generated.h"

class AARProjectileBase;

UCLASS()
class ACTIONROGUELIKE_API UARAction_ProjectileAttack : public UARAction
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = Attack)
	TSubclassOf<AARProjectileBase> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = Attack)
	UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere, Category = Attack)
	FName MuzzleSocketName;

	UPROPERTY(EditAnywhere, Category = Attack)
	UParticleSystem* CastingParticleEffect;

	UPROPERTY(EditAnywhere, Category = Attack)
	float AttackDelay;

	UFUNCTION()
	void AttackDelay_TimerElapsed(ACharacter* InstigatorCharacter);

public:

	UARAction_ProjectileAttack();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;

};
