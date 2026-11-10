// Fill out your copyright notice in the Description page of Project Settings.


#include "KartPlayerController.h"

#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "KartCountdownWidget.h"
#include "KartPawn.h"
#include "KartRaceHUD.h"

void AKartPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// L'UI n'existe que pour le joueur qui joue sur cette machinex
	if (!IsLocalController())
	{
		return;
	}
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (KartMappingContext != nullptr)
		{
			Subsystem->AddMappingContext(KartMappingContext, 0);
		}
	}

	if (CountdownWidgetClass != nullptr)
	{
		CountdownWidget = CreateWidget<UKartCountdownWidget>(this, CountdownWidgetClass);
		if (CountdownWidget != nullptr)
		{
			CountdownWidget->AddToViewport();
		}
	}

	if (RaceHudWidgetClass != nullptr)
	{
		RaceHudWidget = CreateWidget<UKartRaceHUD>(this, RaceHudWidgetClass);
		if (RaceHudWidget != nullptr)
		{
			RaceHudWidget->AddToViewport();
		}
	}
}

void AKartPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInput != nullptr)
	{
		return;
	}
	
	EnhancedInput->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &AKartPlayerController::OnThrottle);
	EnhancedInput->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &AKartPlayerController::OnThrottle);
	
	EnhancedInput->BindAction(SteerAction, ETriggerEvent::Triggered, this, &AKartPlayerController::OnSteer);
	EnhancedInput->BindAction(SteerAction, ETriggerEvent::Completed, this, &AKartPlayerController::OnSteerReleased);
	
	EnhancedInput->BindAction(DriftAction, ETriggerEvent::Started, this, &AKartPlayerController::OnDriftPressed);
	EnhancedInput->BindAction(DriftAction, ETriggerEvent::Completed, this, &AKartPlayerController::OnDriftReleased);
	
	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AKartPlayerController::OnJumpPressed);
	EnhancedInput->BindAction(UseItemAction, ETriggerEvent::Started, this, &AKartPlayerController::OnUseItemPressed);
}

AKartPawn* AKartPlayerController::GetKart() const
{
	return GetPawn<AKartPawn>();
}

void AKartPlayerController::OnThrottle(const FInputActionValue& Value)
{
	if (AKartPawn* Kart = GetKart())
	{
		Kart->SetThrottleInput(Value.Get<float>());
	}
}

void AKartPlayerController::OnThrottleReleased(const FInputActionValue& Value)
{
	if (AKartPawn* Kart = GetKart())
	{
		Kart->SetThrottleInput(0.f);
	}
}

void AKartPlayerController::OnSteer(const FInputActionValue& Value)
{
	if (AKartPawn* Kart = GetKart())
	{
		Kart->SetSteerInput(Value.Get<float>());
	}
}

void AKartPlayerController::OnSteerReleased(const FInputActionValue& Value)
{
	if (AKartPawn* Kart = GetKart())
	{
		Kart->SetSteerInput(0.f);
	}
}

void AKartPlayerController::OnDriftPressed(const FInputActionValue& Value)
{
	if (AKartPawn* Kart = GetKart())
	{
		Kart->StartDrift();
	}
}

void AKartPlayerController::OnDriftReleased(const FInputActionValue& Value)
{
	if (AKartPawn* Kart = GetKart())
	{
		Kart->EndDrift();
	}
}

void AKartPlayerController::OnJumpPressed(const FInputActionValue& Value)
{
	if (AKartPawn* Kart = GetKart())
	{
		Kart->Jump();
	}
}

void AKartPlayerController::OnUseItemPressed(const FInputActionValue& Value)
{
	if (AKartPawn* Kart = GetKart())
	{
		Kart->UseItem();
	}
}
