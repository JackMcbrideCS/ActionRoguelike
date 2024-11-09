// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, UARAttributeComponent*, OwningComponent, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UARAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UARAttributeComponent();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = Attributes)
	static UARAttributeComponent* GetAttributes(const AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = Attributes)
	static bool IsActorAlive(const AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = Attributes)
	static bool KillActor(AActor* Instigator, AActor* Actor);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attributes)
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = Attributes)
	float Health;
	
public:

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	
	UFUNCTION(BlueprintCallable, Category = Attributes)
	bool ApplyHealthChange(AActor* Instigator, float Delta);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	bool Kill(AActor* Instigator);
};
