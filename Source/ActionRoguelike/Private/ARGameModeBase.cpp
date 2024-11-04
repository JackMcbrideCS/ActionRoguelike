// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGameModeBase.h"

#include "ARAttributeComponent.h"
#include "EngineUtils.h"
#include "AI/ARAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

void AARGameModeBase::OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot EQS Query Failed!"));
		return;
	}

	int32 LivingBotCount = 0;
	for (TActorIterator<AARAICharacter> It(GetWorld()); It; ++It)
	{
		const AARAICharacter* AICharacter = *It;
		const UARAttributeComponent* AttributeComponent = AICharacter->GetComponentByClass<UARAttributeComponent>();

		if (!ensure(AttributeComponent))
		{
			continue;
		}
		
		if (AttributeComponent->GetHealth() <= 0.0f)
		{
			continue;
		}
		
		LivingBotCount++;			
	}

	if (!ensure(DifficultyCurve))
	{
		return;
	}

	const float MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	if (LivingBotCount >= FMath::Floor(MaxBotCount))
	{
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
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (!ensure(QueryInstance))
	{
		return;
	}
	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AARGameModeBase::OnSpawnBotQueryCompleted);
}

AARGameModeBase::AARGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void AARGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AARGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}
