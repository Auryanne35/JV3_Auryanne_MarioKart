// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KartPlayerController.generated.h"

class UKartCountdownWidget;
class UKartRaceHUD;
class UInputMappingContext;
class UInputAction;
class AKartPawn;
struct FInputActionValue;

// Controller du joueur : crée l'UI de la course et transforme les touches du joueur en commande pour son kart
UCLASS()
class MARIOKART_API AKartPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	// Widget du compte à rebours à afficher (mettre WBP_Countdown ici dans BP_KartPlayerController)
	UPROPERTY(EditAnywhere, Category = "Kart|UI")
	TSubclassOf<UKartCountdownWidget> CountdownWidgetClass;

	// Widget du chrono et des tours (mettre WBP_RaceHud ici dans BP_KartPlayerController)
	UPROPERTY(EditAnywhere, Category = "Kart|UI")
	TSubclassOf<UKartRaceHUD> RaceHudWidgetClass;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Input")
	TObjectPtr<UInputMappingContext> KartMappingContext;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Input")
	TObjectPtr<UInputAction> ThrottleAction;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Input")
	TObjectPtr<UInputAction> SteerAction;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Input")
	TObjectPtr<UInputAction> DriftAction;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Input")
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Input")
	TObjectPtr<UInputAction> UseItemAction;

private:
	
	void OnThrottle(const FInputActionValue& Value);
	void OnThrottleReleased(const FInputActionValue& Value);
	void OnSteer(const FInputActionValue& Value);
	void OnSteerReleased(const FInputActionValue& Value);
	void OnDriftPressed(const FInputActionValue& Value);
	void OnDriftReleased(const FInputActionValue& Value);
	void OnJumpPressed(const FInputActionValue& Value);
	void OnUseItemPressed(const FInputActionValue& Value);
	
	AKartPawn* GetKart() const;
	
	UPROPERTY()
	TObjectPtr<UKartCountdownWidget> CountdownWidget;

	UPROPERTY()
	TObjectPtr<UKartRaceHUD> RaceHudWidget;
};