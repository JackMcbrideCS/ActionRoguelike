// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGameModeBase.h"

#include "Attributes/ARAttributeComponent.h"
#include "ARCharacter.h"
#include "EngineUtils.h"
#include "AI/ARAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("ar.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

void AARGameModeBase::OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot EQS Query Failed!"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot EQS Query Returned NO Locations!"));
		return;
	}

	GetWorld()->SpawnActor<AActor>(BotClass, Locations[0], FRotator::ZeroRotator);
}

void AARGameModeBase::SpawnBotTimerElapsed()
{
	if (!CanSpawnBot())
	{
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (!ensure(QueryInstance))
	{
		return;
	}
	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AARGameModeBase::OnSpawnBotQueryCompleted);
}

void AARGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (!ensure(Controller))
	{
		return;
	}

	Controller->UnPossess();
	RestartPlayer(Controller);
}

bool AARGameModeBase::CanSpawnBot()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		return false;
	}
	
	int32 LivingBotCount = 0;
	for (TActorIterator<AARAICharacter> It(GetWorld()); It; ++It)
	{
		if (!UARAttributeComponent::IsActorAlive(*It))
		{
			continue;
		}
		
		LivingBotCount++;			
	}

	if (!ensure(DifficultyCurve))
	{
		return false;
	}

	const float MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	if (LivingBotCount >= FMath::Floor(MaxBotCount))
	{
		return false;
	}

	return true;
}

AARGameModeBase::AARGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	RespawnDelay = 2.0f;
}

void AARGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AARGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void AARGameModeBase::OnActorKilled(AActor* KillInstigator, AActor* Killed)
{
	AARCharacter* Player = Cast<AARCharacter>(Killed);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, TEXT("RespawnPlayerElapsed"), Player->GetController());
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, TimerDelegate, RespawnDelay, false);
	}
}

void AARGameModeBase::KillAllBots()
{
	for (TActorIterator<AARAICharacter> It(GetWorld()); It; ++It)
	{
		if (!UARAttributeComponent::IsActorAlive(*It))
		{
			continue;
		}
		
		UARAttributeComponent::KillActor(this, *It);
	}
}
