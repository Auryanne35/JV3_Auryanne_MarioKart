// Fill out your copyright notice in the Description page of Project Settings.


#include "KartRaceHUD.h"

#include "Components/TextBlock.h"
#include "KartGameMode.h"
#include "KartPawn.h"

void UKartRaceHUD::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetVisibility(ESlateVisibility::Collapsed);
	
	if (AKartGameMode* GameMode = GetWorld()->GetAuthGameMode<AKartGameMode>())
	{
		GameMode->OnRaceStarted.AddUniqueDynamic(this, &UKartRaceHUD::HandleRaceStarted);
		
		if (GameMode->IsRaceStarted())
		{
			HandleRaceStarted();
		}
	}
}

void UKartRaceHUD::NativeDestruct()
{
	if (UWorld* World =GetWorld())
	{
		if (AKartGameMode* GameMode = World->GetAuthGameMode<AKartGameMode>())
		{
			GameMode->OnRaceStarted.RemoveDynamic(this, &UKartRaceHUD::HandleRaceStarted);
		}
	}
	
	Super::NativeDestruct();
}

void UKartRaceHUD::HandleRaceStarted()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UKartRaceHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	const AKartGameMode* GameMode = GetWorld()->GetAuthGameMode<AKartGameMode>();
	
	const AKartPawn* Kart = GetOwningPlayerPawn<AKartPawn>();
	
	if (GameMode == nullptr || Kart == nullptr)
	{
		return;
	}
	
	if (!GameMode->IsRaceStarted())
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	if (Kart->HasFinishedRace())
	{
		TimerText->SetText(AKartGameMode::FormatRaceTime(Kart->GetFinishedTime()));
		LapText->SetText(FinishedText);
	}
	
	else
	{
		TimerText->SetText(AKartGameMode::FormatRaceTime(GameMode->GetRaceTime()));
		LapText->SetText(FText::Format(NSLOCTEXT("Kart", "LapFormat", "Tour {0}/{1}"),FText::AsNumber(Kart->GetCurrentLap()), FText::AsNumber(GameMode->GetLapCount())));
	}
}
