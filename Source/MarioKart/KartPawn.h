// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "KartPawn.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class MARIOKART_API AKartPawn : public APawn
{
	GENERATED_BODY()

public:
	//Sets default values for this pawn's properties
	AKartPawn();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, category = "Kart")
	void SetCanMove(bool bInCanMove);
	
	UFUNCTION(BlueprintPure, category = "Kart")
	bool CanMove() const { return bCanMove; }

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//Permet de changer le controller pendant le jeu
	virtual void NotifyControllerChanged() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|components")
	TObjectPtr<USphereComponent> CollisionSphere;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|components")
	TObjectPtr<UStaticMeshComponent> BodyMesh;
	
	//Les 4 roues
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|components")
	TObjectPtr<UStaticMeshComponent> WheelFrontLeft;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|components")
	TObjectPtr<UStaticMeshComponent> WheelFrontRight;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|components")
	TObjectPtr<UStaticMeshComponent> WheelRearLeft;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|components")
	TObjectPtr<UStaticMeshComponent> WheelRearRight;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|components")
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|components")
	TObjectPtr<UCameraComponent> Camera;
	
	//Input
	
	UPROPERTY(EditAnywhere, Category = "Kart|input")
	TObjectPtr<UInputMappingContext> KartMappingContext;
	
	UPROPERTY(EditAnywhere, Category = "Kart|input")
	TObjectPtr<UInputAction> ThrottleAction;
	
	UPROPERTY(EditAnywhere, Category = "Kart|input")
	TObjectPtr<UInputAction> SteerAction;
	
	UPROPERTY(EditAnywhere, Category = "Kart|input")
	TObjectPtr<UInputAction> DriftAction;
	
	UPROPERTY(EditAnywhere, Category = "Kart|input")
	TObjectPtr<UInputAction> JumpAction;
	
	//Réglages: déplacement
	
	UPROPERTY(EditAnywhere, Category = "Kart|Movement")
	float MaxForwardSpeed = 2500.f;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Movement")
	float MaxReverseSpeed = 800.f;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Movement")
	float Acceleration = 1500.f;
	
	//Décélération quand on appuie dans le sens opposé
	UPROPERTY(EditAnywhere, Category = "Kart|Movement")
	float BrakeDeceleration = 3000.f;
	
	//décélération quand on arrête d'appuyer
	UPROPERTY(EditAnywhere, Category = "Kart|Movement")
	float CoastDeceleration = 800.f;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Movement")
	float TurnRate = 100.f;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Movement")
	float MinSpeedToTurn = 50.f;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Movement", meta = (ClampMin = "0", ClampMax = "1"))
	float WallSpeedFactor = 0.3f;
	
	//Réglage: gravité
	
	UPROPERTY(EditAnywhere, Category = "Kart|Gravity")
	float GravityStrenght = 2500.f;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Gravity")
	float GroundTraceDistance = 80.f;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Gravity")
	float RideHeight = 45.f;
	
	//Permet de décoller quand une pente est plus élevé
	UPROPERTY(EditAnywhere, Category = "Kart|Gravity")
	float GroundStickDistance = 30.f;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Gravity")
	float GroundAlignSpeed = 8.f;
	
	//Distance entre l'avant et l'arrière du véhicule
	UPROPERTY(EditAnywhere, Category = "Kart|Gravity")
	float GroundProbeHalfLenght = 50.f;
	
	//Distance entre les roue droite et gauche
	UPROPERTY(EditAnywhere, Category = "Kart|Gravity")
	float GroundProbeHalfWidth = 50.f;
	
	//Réglage: Saut
	
	UPROPERTY(EditAnywhere, Category = "Kart|Jump")
	float JumpVelocity = 450.f;
	
	//Réglage: drift et boost
	
	UPROPERTY(EditAnywhere, Category = "Kart|Drift")
	float DriftMinSpeed = 1000.f;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Drift")
	float DriftTurnMultiplier = 1.4f;
	
	//Angle visuel du chassis pendant le drift
	UPROPERTY(EditAnywhere, Category = "Kart|Drift")
	float DriftSlideAngle = 25.f;
	
	//Temps pour entrer complètement dans le drift
	UPROPERTY(EditAnywhere, Category = "Kart|Drift", meta = (ClampMin = "0.01"))
	float DriftEnterTime = 0.4f;
	
	//Vitesse à laquelle le chassi rejoint son angle
	UPROPERTY(EditAnywhere, Category = "Kart|Drift")
	float DriftSlideInterSpeed = 6.f;
	
	//Temps de drift pour obtenir le mini-turbo (bleu)
	UPROPERTY(EditAnywhere, Category = "Kart|Drift")
	float MiniTurboChargeTime = 1.f;
	
	//Temps de drift pour obtenir le médium-turbo (orange)
	UPROPERTY(EditAnywhere, Category = "Kart|Drift")
	float SuperTurboChargeTime = 2.f;
	
	//Temps pour que dure le turbo minimum
	UPROPERTY(EditAnywhere, Category = "Kart|Drift")
	float MiniTurboDuration = 0.6f;
	
	//Temps que dure le turbo maximum
	UPROPERTY(EditAnywhere, Category = "Kart|Drift")
	float SuperTurboDuration = 1.2f;
	
	//Boost maximum ajouter
	UPROPERTY(EditAnywhere, Category = "Kart|Drift")
	float BoostSpeed = 3500.f;
	
	//Réglage: Visuel
	
	UPROPERTY(EditAnywhere, Category = "Kart|Visual")
	float WheelRadius = 20;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Visual")
	float MaxWheelSteerAngle = 30.f;
	
	//FOV à l'arrêt
	UPROPERTY(EditAnywhere, Category = "Kart|Camera")
	float BaseFieldOfView = 90.f;
	
	//FOV à vitesse maximal
	UPROPERTY(EditAnywhere, Category = "Kart|Camera")
	float MaxSpeedFieldOfView = 100.f;
	
	//FOV pendant le boost
	UPROPERTY(EditAnywhere, Category = "Kart|Camera")
	float BoostFieldOfView = 100.f;
	
	UPROPERTY(EditAnywhere, Category = "Kart|Camera", meta = (ClampMin = "0", ClampMax = "1"))
	float CameraDriftFollow = 1.f;
	
	//Vitesse à laquelle la caméra rattrape l'angle du châssis 
	UPROPERTY(EditAnywhere, Category = "Kart|Camera")
	float CameraDriftInterSpeed = 3.f;
	
	// EN-dessous de cette pente, la caméra ne s'incline pas du tout
	UPROPERTY(EditAnywhere, Category = "Kart|Camera", meta = (ClampMin = "0", ClampMax = "90"))
	float CameraSlopeDeadZone = 10.f;
	
	//Vitesse à laquelle la caméra s'incline dans les grandes pentes
	UPROPERTY(EditAnywhere, Category = "Kart|Camera")
	float CameraSlopeFollowSpeed = 3.f;
	
	//Vitesse à laquelle le FOV change
	UPROPERTY(EditAnywhere, Category = "Kart|Camera")
	float FieldOfViewInterSpeed = 4.f;
	
	//Affiche la vitesse et l'état du drift à l'écran
	UPROPERTY(EditAnywhere, Category = "Kart|Visual")
	bool bShowDebug = true;
	
	//état
	
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	float CurrentSpeed = 0.f;
	
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	float VerticalSpeed = 0.f;
	
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	float ThrottleInput = 0.f;
	
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	float SteerInput = 0.f;
	
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	bool bIsGrounded = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	bool bIsDrifting = false;
	
	// -1 = gauche, 1 = droite
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	int32 DriftDirection = 0;
	
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	float DriftChargeTime = 0.f;
	
	//0 = début du drift, 1 = drift
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	float DriftBlend = 0.f;
	
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	float BoostTimeRemaining = 0.f;
	
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	float WheelSpinAngle = 0.f;
	
	UPROPERTY(VisibleAnywhere, Category = "Kart|State")
	FVector GroundNormal = FVector::UpVector;
	
	//Faux pendant le compte à rebours
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kart|State")
	bool bCanMove = true;
	
private:
	
	//Fonction appelées par l'input
	void OnThrottle (const FInputActionValue & Value);
	void OnThrottleReleased (const FInputActionValue & Value);
	void OnSteer (const FInputActionValue & Value);
	void OnSteerReleased (const FInputActionValue & Value);
	void OnDriftPressed (const FInputActionValue & Value);
	void OnDriftReleased (const FInputActionValue & Value);
	void OnJumpPressed (const FInputActionValue & Value);
	
	//étapes du tick
	void UpdateGround(float DeltaTime);
	void UpdateSpeed(float DeltaTime);
	void UpdateSteering(float DeltaTime);
	void UpdateDrift(float DeltaTime);
	void UpdateBoost(float DeltaTime);
	void MoveKart(float DeltaTime);
	void AlignToGround(float DeltaTime);
	void UpdateVisuals(float DeltaTime);
	void UpdateCameraFieldOfView(float DeltaTime);
	void ShowDebugInfo() const;
	
	int32 TraceGroundUnderWheels(FVector& OutGroundPoint, FVector& OutGroundNormal) const;
	
	//Jump
	void Jump();
	
	//Drift et boost
	void StartDrift();
	void EndDrift();
	void StartBoost(float Duration);
	float GetCurrentMaxSpeed() const;
	
	//Visuel des roues
	void SetWheelRotation(UStaticMeshComponent* Wheel, const FRotator& BaseRotation, float SteerAngle) const;
	
	//Rotation de départ placé dans le blueprint
	FRotator BodyBaseRotation;
	FRotator SpringArmBaseRotation;
	FRotator WheelFrontLeftBaseRotation;
	FRotator WheelFrontRightBaseRotation;
	FRotator WheelRearLeftBaseRotation;
	FRotator WheelRearRightBaseRotation;
	
	//Angle de l acaméra pendant le drift
	float CameraSlideYaw = 0.f;
	
	//Inclinaison de la caméra dans les grandes pentes
	float CameraSlopePitch = 0.f;
	
	//Nombre de fois où la sphère a touche le sol en roulant
	int32 SphereFloorHitCount = 0;
};
