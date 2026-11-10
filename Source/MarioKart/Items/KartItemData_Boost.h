// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/KartItemData.h"
#include "KartItemData_Boost.generated.h"

/**
 * 
 */
UCLASS()
class MARIOKART_API UKartItemData_Boost : public UKartItemData
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Boost")
	float BoostDuration = 1.5f;
	
	virtual bool Activate(AKartPawn* Kart) const override;
};
