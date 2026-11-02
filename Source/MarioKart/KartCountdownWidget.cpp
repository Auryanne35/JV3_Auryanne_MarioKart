// Fill out your copyright notice in the Description page of Project Settings.


#include "KartCountdownWidget.h"

#include "Components/TextBlock.h"
#include "KartGameMode.h"
#include "TimerManager.h"

void UKartCountdownWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetVisibility(ESlateVisibility::Collapsed);
	
	if (AKartGameMode* GameMode = GetWorld()->GetAuthGameMode<AKartGameMode>())
	{
		GameMode->OnCountdownTick.AddUniqueDynamic(this, &UKartCountdownWidget::HandleCountdownTick);
		
		if (!GameMode->IsRaceStarted() && GameMode->GetCountdownNumber() > 0)
		{
			HandleCountdownTick(GameMode->GetCountdownNumber());
		}
	}
}

void UKartCountdownWidget::NativeDestruct()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(HideTimer);
		
		if (AKartGameMode* GameMode = World->GetAuthGameMode<AKartGameMode>())
		{
			GameMode->OnCountdownTick.RemoveDynamic(this, &UKartCountdownWidget::HandleCountdownTick);
		}
	}
	
	Super::NativeDestruct();
}

void UKartCountdownWidget::HandleCountdownTick(int32 Number)
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	GetWorld()->GetTimerManager().ClearTimer(HideTimer);
	
	if (Number >0)
	{
		CountdownText->SetText(FText::AsNumber(Number));
	}
	else
	{
		CountdownText->SetText(GoText);
		GetWorld()->GetTimerManager().SetTimer(HideTimer, this, &UKartCountdownWidget::HideCountdown, FMath::Max(GoDisplayTime, 0.01f), false);
	}
	
	OnCountdownNumberShow(Number);
}

void UKartCountdownWidget::HideCountdown()
{
	//Afficher à l'écran/créer mais pas visible
	SetVisibility(ESlateVisibility::Collapsed);
}