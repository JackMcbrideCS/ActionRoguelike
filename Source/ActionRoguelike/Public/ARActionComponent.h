// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "ARActionComponent.generated.h"


class UARAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UARActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tags)
	FGameplayTagContainer ActiveGameplayTags;
	
	UARActionComponent();

	UFUNCTION(BlueprintCallable, Category = Actions)
	void AddAction(TSubclassOf<UARAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = Actions)
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = Actions)
	bool StopActionByName(AActor* Instigator, FName ActionName);

protected:

	UPROPERTY()
	TArray<UARAction*> Actions;

	UPROPERTY(EditAnywhere, Category = Actions)
	TArray<TSubclassOf<UARAction>> DefaultActions;

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
