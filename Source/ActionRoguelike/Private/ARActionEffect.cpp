// Fill out your copyright notice in the Description page of Project Settings.


#include "ARActionEffect.h"

#include "ARActionComponent.h"

UARActionEffect::UARActionEffect()
{
	bAutoStart = true;
}

void UARActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	
}

void UARActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Duration, Delegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Period, Delegate, Period, true);
	}
}

void UARActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_Period) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
	
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Period);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Duration);

	Super::StopAction_Implementation(Instigator);
	UARActionComponent* ActionComponent = GetOwningComponent();
	if (ActionComponent)
	{
		ActionComponent->RemoveAction(this);
	}
}
