// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KartRaceHUD.generated.h"

class UTextBlock;
/**
 widget de course: affiche le chrono et le tours en cours
 Il est caché pendant le 3,2,1 et apparaît au go
 */
UCLASS()
class MARIOKART_API UKartRaceHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimerText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LapText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kart|Race")
	FText FinishedText = NSLOCTEXT("Kart", "Finished", "FINI !");
	
private:
	UFUNCTION()
	void HandleRaceStarted();
};
