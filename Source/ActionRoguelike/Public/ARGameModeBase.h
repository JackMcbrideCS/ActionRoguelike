// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARSaveGame.h"
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

	UPROPERTY()
	UARSaveGame* CurrentSaveGame;

	FTimerHandle TimerHandle_SpawnBots;
	FString SlotName;

	UFUNCTION()
	void OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	bool CanSpawnBot();
	
public:
	
	AARGameModeBase();
	
	UFUNCTION(BlueprintCallable, Category = SaveGame)
	void WriteSaveGame();

	UFUNCTION(Exec)
	void KillAllBots();
	
	void LoadSaveGame();

	virtual void StartPlay() override;
	virtual void OnActorKilled(AActor* KillInstigator, AActor* Killed);
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};
