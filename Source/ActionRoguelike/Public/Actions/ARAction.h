// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "ARAction.generated.h"

class UARAttributeComponent;
class UARActionComponent;
/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API UARAction : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = Action)
	bool bAutoStart;

	UPROPERTY(EditDefaultsOnly, Category = Action)
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category = Action)
	void StartAction(AActor* Instigator);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Action)
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = Action)
	bool CanStart(AActor* Instigator) const;

	UFUNCTION(BlueprintNativeEvent, Category = Action)
	bool IsRunning() const;
	
	virtual UWorld* GetWorld() const override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = Tags)
	FGameplayTagContainer GrantsTags;
	
	UPROPERTY(EditDefaultsOnly, Category = Tags)
	FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing = "OnRep_IsRunning")
	bool bIsRunning;

	UFUNCTION()
	void OnRep_IsRunning();
	
	UFUNCTION(BlueprintCallable, Category = Action)
	UARActionComponent* GetOwningComponent() const;

	UFUNCTION(BlueprintCallable, Category = Attributes)
	UARAttributeComponent* GetOwnerAttributes() const;

	UFUNCTION(BlueprintCallable, Category = Attributes)
	AActor* GetOwner() const;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }
};
