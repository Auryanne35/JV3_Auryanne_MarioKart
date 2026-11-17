// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/KartItemProjectile.h"

static constexpr  float ProjectileFloorZ = 0.7f;

AKartItemProjectile::AKartItemProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	InitialLifeSpan = 10.f;
}

void AKartItemProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	MoveDirection = GetActorForwardVector().GetSafeNormal2D();
	BouncesLeft = MaxBounces;
}

void AKartItemProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	FHitResult GroundHit;
	FVector Direction = MoveDirection;
	
	if (FindGround(HoverHeight + GroundStickDistance, GroundHit))
	{
		VerticalSpeed = 0.f;
		SetActorLocation( GroundHit.ImpactPoint + FVector::UpVector * HoverHeight);
		
		Direction = FVector::VectorPlaneProject(MoveDirection, GroundHit.ImpactNormal).GetSafeNormal();
	}
	
	else
	{
		VerticalSpeed -= GravityStrenght * DeltaTime;
	}
	
	const FVector Delta = Direction * Speed * DeltaTime + FVector::UpVector * VerticalSpeed * DeltaTime;
	
	FHitResult Hit;
	
	AddActorWorldOffset(Delta, true, &Hit);
	
	if (!Hit.bBlockingHit)
	{
		return;
	}
	
	if (Hit.Normal.Z >= ProjectileFloorZ)
	{
		AddActorWorldOffset(Delta * (1.f - Hit.Time), false);
		return;
	}
	
	if (BouncesLeft <= 0)
	{
		Destroy();
		return;
	}
	--BouncesLeft;
	
	// MirrorByVector renvoie la direction "en miroir" par rapport au mur, comme une balle de billard :
	// la partie qui rentre dans le mur est inversée, la partie qui longe le mur est gardée
	// GetSafeNormal2D enlève la hauteur (Z) et remet la longueur à 1 : on reste à l'horizontale
	MoveDirection = MoveDirection.MirrorByVector(Hit.Normal).GetSafeNormal2D();
	SetActorRotation(MoveDirection.Rotation());
}