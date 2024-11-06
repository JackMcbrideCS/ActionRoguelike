// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "ARGameModeBase.generated.h"

class UEnvQueryInstanceBlueprintWrapper;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = AI)
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	TSubclassOf<AActor> BotClass;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, Category = Player)
	float RespawnDelay;

	FTimerHandle TimerHandle_SpawnBots;

	UFUNCTION()
	void OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	bool CanSpawnBot();
	
public:
	
	AARGameModeBase();

	virtual void StartPlay() override;
	virtual void OnActorKilled(AActor* KillInstigator, AActor* Killed);

	UFUNCTION(Exec)
	void KillAllBots();
};
