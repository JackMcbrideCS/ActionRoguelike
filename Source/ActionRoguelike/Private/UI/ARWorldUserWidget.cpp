// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ARWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

void UARWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();
		UE_LOG(LogTemp, Warning, TEXT("Attached Actor is no longer valid, removing World Widget"));
		return;
	}
	
	if (!ensure(ParentSizeBox))
	{
		return;
	}
	
	FVector2D ScreenPosition;
	if(!UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition))
	{
		return;
	}

	const float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(GetOwningPlayer());
	ScreenPosition /= ViewportScale;
	ParentSizeBox->SetRenderTranslation(ScreenPosition);
}
