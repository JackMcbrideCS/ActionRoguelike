// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGameModeBase.h"

#include "Attributes/ARAttributeComponent.h"
#include "ARCharacter.h"
#include "ARGameplayInterface.h"
#include "ARPlayerState.h"
#include "ARSaveGame.h"
#include "EngineUtils.h"
#include "AI/ARAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

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
	SlotName = TEXT("SaveGame01");
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

void AARGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}

void AARGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	AARPlayerState* PlayerState = NewPlayer->GetPlayerState<AARPlayerState>();
	if (!ensure(PlayerState))
	{
		return;
	}

	PlayerState->LoadPlayerState(CurrentSaveGame);
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

void AARGameModeBase::WriteSaveGame()
{
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AARPlayerState* PlayerState = Cast<AARPlayerState>(GameState->PlayerArray[i]);
		if (!PlayerState)
		{
			return;
		}

		PlayerState->SavePlayerState(CurrentSaveGame);
		break;
	}

	CurrentSaveGame->SavedActors.Empty();
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<UARGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.SaveFromActor(Actor);
		CurrentSaveGame->SavedActors.Add(ActorData);
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void AARGameModeBase::LoadSaveGame()
{
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UARSaveGame>(UGameplayStatics::CreateSaveGameObject(UARSaveGame::StaticClass()));
		return;
	}
	
	CurrentSaveGame = Cast<UARSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (!ensure(CurrentSaveGame))
	{
		return;
	}
	
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<UARGameplayInterface>())
		{
			continue;
		}

		for (const FActorSaveData& ActorData : CurrentSaveGame->SavedActors)
		{
			if (ActorData.ActorName == Actor->GetName())
			{
				ActorData.LoadToActor(Actor);
				IARGameplayInterface::Execute_OnActorLoaded(Actor);
				break;
			}
		}
	}
}