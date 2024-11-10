// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, UARAttributeComponent*, OwningComponent, float, NewValue, float, Delta);

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = Attributes)
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = Attributes)
	float Health;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attributes)
	float MaxRage;

	UPROPERTY(BlueprintReadOnly, Category = Attributes)
	float Rage;

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastHealthChanged(AActor* Instigator, float NewHealth, float Delta);

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRageChanged(AActor* Instigator, float NewRage, float Delta);
	
public:

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;
	
	UFUNCTION(BlueprintCallable, Category = Attributes)
	bool ApplyHealthChange(AActor* Instigator, float Delta);

	UFUNCTION(BlueprintCallable, Category = Attributes)
	bool ApplyRageChange(AActor* Instigator, float Delta);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxRage() const;

	UFUNCTION(BlueprintCallable)
	float GetRage() const;

	bool Kill(AActor* Instigator);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
