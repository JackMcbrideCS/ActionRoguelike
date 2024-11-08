// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "ARAction.generated.h"

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

	bool bIsRunning;
	
	UFUNCTION(BlueprintCallable, Category = Action)
	UARActionComponent* GetOwningComponent() const;
};
