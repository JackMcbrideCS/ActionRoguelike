// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "ARSaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FString ActorName;

	UPROPERTY()
	FTransform ActorTransform;

	UPROPERTY()
	TArray<uint8> ByteData;

	void SaveFromActor(AActor* Actor)
	{
		ActorName = Actor->GetName();
		ActorTransform = Actor->GetActorTransform();

		FMemoryWriter MemoryWriter(ByteData);
		FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
		Archive.ArIsSaveGame = true;
		Actor->Serialize(Archive);
	}
	
	void LoadToActor(AActor* Actor) const
	{
		Actor->SetActorTransform(ActorTransform);

		FMemoryReader MemoryReader(ByteData);
		FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
		Archive.ArIsSaveGame = true;
		Actor->Serialize(Archive);
	}
};

UCLASS()
class ACTIONROGUELIKE_API UARSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 Credits;

	UPROPERTY()
	TArray<FActorSaveData> SavedActors;
};
