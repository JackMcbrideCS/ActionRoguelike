// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ARPlayerController.generated.h"

struct FInputActionValue;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);

UCLASS()
class ACTIONROGUELIKE_API AARPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY()
	UUserWidget* PauseMenuInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction{nullptr};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* QuickSaveAction{nullptr};
	
	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBeginPlayingState();

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

	virtual void SetPawn(APawn* InPawn) override;
	virtual void BeginPlayingState() override;
	virtual void SetupInputComponent() override;

	void OnPause(const FInputActionValue& Value);
	void OnQuickSave(const FInputActionValue& Value);
};
