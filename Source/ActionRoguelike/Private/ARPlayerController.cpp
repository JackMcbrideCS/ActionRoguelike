// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPlayerController.h"

#include "ARGameModeBase.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AARPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	OnPawnChanged.Broadcast(InPawn);
}

void AARPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
	BlueprintBeginPlayingState();
}

void AARPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	if (!ensure(EnhancedInputComponent))
	{
		return;
	}
	
	EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AARPlayerController::OnPause);
	EnhancedInputComponent->BindAction(QuickSaveAction, ETriggerEvent::Started, this, &AARPlayerController::OnQuickSave);
}

void AARPlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		return;
	}
	
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (!ensure(PauseMenuInstance))
	{
		return;
	}

	PauseMenuInstance->AddToViewport(100);
	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());
}

void AARPlayerController::OnPause(const FInputActionValue& Value)
{
	TogglePauseMenu();
}

void AARPlayerController::OnQuickSave(const FInputActionValue& Value)
{
	Cast<AARGameModeBase>(UGameplayStatics::GetGameMode(this))->WriteSaveGame();
}
