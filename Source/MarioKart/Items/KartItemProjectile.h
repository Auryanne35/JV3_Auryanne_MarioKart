// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/KartItemHazard.h"
#include "KartItemProjectile.generated.h"

/**
 * 
 */
UCLASS()
class MARIOKART_API AKartItemProjectile : public AKartItemHazard
{
	GENERATED_BODY()
	
public:
	AKartItemProjectile();
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kart|Projectile")
	float Speed = 4000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kart|Projectile")
	int32 MaxBounces = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kart|Projectile")
	float GravityStrenght = 2500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kart|Projectile")
	float GroundStickDistance = 30.f;
	
	private:
	FVector MoveDirection = FVector::ForwardVector;
	float VerticalSpeed = 0.f;
	int32 BouncesLeft = 0;
	
};
