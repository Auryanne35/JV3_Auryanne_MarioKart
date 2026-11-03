// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KartPlayerController.generated.h"

class UKartCountdownWidget;
class UKartRaceHUD;

// Controller du joueur : crée l'UI de la course (seulement pour le joueur local)
UCLASS()
class MARIOKART_API AKartPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	// Widget du compte à rebours à afficher (mettre WBP_Countdown ici dans BP_KartPlayerController)
	UPROPERTY(EditAnywhere, Category = "Kart|UI")
	TSubclassOf<UKartCountdownWidget> CountdownWidgetClass;

	// Widget du chrono et des tours (mettre WBP_RaceHud ici dans BP_KartPlayerController)
	UPROPERTY(EditAnywhere, Category = "Kart|UI")
	TSubclassOf<UKartRaceHUD> RaceHudWidgetClass;

private:
	UPROPERTY()
	TObjectPtr<UKartCountdownWidget> CountdownWidget;

	UPROPERTY()
	TObjectPtr<UKartRaceHUD> RaceHudWidget;
};