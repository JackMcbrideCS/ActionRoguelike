// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ARBTService_HealthThreshold.generated.h"

UENUM()
namespace EBTServiceHealthThreshold
{
	enum Type : int
	{
		Greater,
		GreaterEqual,
		Less,
		LessEqual
	};
}

UCLASS()
class ACTIONROGUELIKE_API UARBTService_HealthThreshold : public UBTService
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector HealthThresholdKey;
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EBTServiceHealthThreshold::Type> Mode;

	UPROPERTY(EditAnywhere)
	float Threshold;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
