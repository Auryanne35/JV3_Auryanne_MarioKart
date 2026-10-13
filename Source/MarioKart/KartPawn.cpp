// Fill out your copyright notice in the Description page of Project Settings.


#include "KartPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
//#include "KartGameMode.h"

//Marge sous les roues pour considérer que le kart touche le sol
static constexpr float GroundSnapTolerance = 5.f;

//Une surface dont la normale a un Z plus grand que ça est un sol, sinon c'est un mur
static constexpr float WalkableFloorZ = 0.7f;
// Sets default values
AKartPawn::AKartPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetSphereRadius(35.f);
	CollisionSphere->SetCollisionProfileName(TEXT("PAWN"));
	CollisionSphere->SetSimulatePhysics(false);
	RootComponent = CollisionSphere;
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(CollisionSphere);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	//Les roues sont attechées au châssi
	WheelFrontLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelFrontLeft"));
	WheelFrontLeft->SetupAttachment(BodyMesh);
	WheelFrontLeft->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	WheelFrontRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelRight"));
	WheelFrontRight->SetupAttachment(BodyMesh);
	WheelFrontRight->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	WheelRearLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelLeft"));
	WheelRearLeft->SetupAttachment(BodyMesh);
	WheelRearLeft->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	WheelRearRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelRight"));
	WheelRearRight->SetupAttachment(BodyMesh);
	WheelRearRight->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	//Caméra dérrière et au-dessus du kart
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(CollisionSphere);
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 600.f;
	SpringArm->SocketOffset = FVector(0.f,0.f,100.f);
	
	//Pas de retard en position: à grande vitesse il fait trembler le kart d'avant en arrière à l'écran
	SpringArm->bEnableCameraLag = false;
	
	//Retart en rotation seulement : la caméra suit les virages en douceur
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 6.f;
	SpringArm->bInheritRoll = false;
	
	//La caméra ne copie pas l'inclinaison du kart : on choisit nous-mêmes son angle 
	SpringArm->bInheritPitch = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

}

// Called when the game starts or when spawned
void AKartPawn::BeginPlay()
{
	Super::BeginPlay();
	
	//On retient les rotations placées dans le blueprint pour ne pas les écraser
	BodyBaseRotation = BodyMesh->GetRelativeRotation();
	SpringArmBaseRotation = SpringArm->GetRelativeRotation();
	WheelFrontLeftBaseRotation = WheelFrontLeft->GetRelativeRotation();
	WheelFrontRightBaseRotation = WheelFrontRight->GetRelativeRotation();
	WheelRearLeftBaseRotation = WheelRearLeft->GetRelativeRotation();
	WheelRearRightBaseRotation = WheelRearRight->GetRelativeRotation();
}

void AKartPawn::SetCanMove(bool bInCanMove)
{
	bCanMove = bInCanMove;
}

void AKartPawn::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(KartMappingContext, 0);
		}
	}
}

void AKartPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInput == nullptr)
	{
		return;
	}
	
	EnhancedInput->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &AKartPawn::OnThrottle);
	EnhancedInput->BindAction(ThrottleAction, ETriggerEvent::Completed,this, &AKartPawn::OnThrottleReleased);
	
	EnhancedInput->BindAction(SteerAction,ETriggerEvent::Triggered,this, &AKartPawn::OnSteer);
	EnhancedInput->BindAction(SteerAction,ETriggerEvent::Completed,this, &AKartPawn::OnSteerReleased);
	
	EnhancedInput->BindAction(DriftAction,ETriggerEvent::Triggered,this, &AKartPawn::OnDriftPressed);
	EnhancedInput->BindAction(DriftAction,ETriggerEvent::Completed,this,&AKartPawn::OnDriftReleased);
	
	EnhancedInput->BindAction(JumpAction,ETriggerEvent::Started,this, &AKartPawn::OnJumpPressed);
}

// Called every frame
void AKartPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!bCanMove)
	{
		ThrottleInput = 0.f;
		SteerInput = 0.f;
	}
	
	UpdateGround(DeltaTime);
	UpdateBoost(DeltaTime);
	UpdateSpeed(DeltaTime);
	UpdateSteering(DeltaTime);
	UpdateDrift(DeltaTime);
	MoveKart(DeltaTime);
	AlignToGround(DeltaTime);
	UpdateVisuals(DeltaTime);
	UpdateCameraFieldOfView(DeltaTime);
	
	if (bShowDebug)
	{
		ShowDebugInfo();
	}
}

void AKartPawn::OnThrottle(const FInputActionValue& Value)
{
	ThrottleInput = Value.Get<float>();
}

void AKartPawn::OnThrottleReleased(const FInputActionValue& Value)
{
	ThrottleInput = 0.f;
}

void AKartPawn::OnSteer(const FInputActionValue& Value)
{
	SteerInput = Value.Get<float>();
}

void AKartPawn::OnSteerReleased(const FInputActionValue& Value)
{
	SteerInput = 0.f;
}

void AKartPawn::OnDriftPressed(const FInputActionValue& Value)
{
	if (bCanMove)
	{
		StartDrift();
	}
}

void AKartPawn::OnDriftReleased(const FInputActionValue& Value)
{
	EndDrift();
}

void AKartPawn::OnJumpPressed(const FInputActionValue& Value)
{
	if (bCanMove)
	{
		Jump();
	}
}

void AKartPawn::UpdateGround(float DeltaTime)
{
	FVector GroundPoint;
	FVector WheelsNormal;
	
	const int32 WheelsOnFloor = TraceGroundUnderWheels(GroundPoint, WheelsNormal);
	const bool bHitFloor = WheelsOnFloor > 0;
	
	const float HeightAboveGround = FVector::DotProduct(GetActorLocation() - GroundPoint, GetActorUpVector());
	
	const float MaxGroundHeight = RideHeight + (bIsGrounded ? GroundStickDistance : GroundSnapTolerance);
	bIsGrounded = bHitFloor && HeightAboveGround <= MaxGroundHeight && VerticalSpeed <= 0.f;
	
	// ? = if 
	GroundNormal = bHitFloor ? WheelsNormal : FVector::UpVector;
	
	if (bIsGrounded)
	{
		VerticalSpeed = 0.f;
		
		const float HeightError = HeightAboveGround - RideHeight;
		AddActorWorldOffset(-GetActorUpVector() * HeightError, true);
	}
	else
	{
		VerticalSpeed -= GravityStrenght * DeltaTime;
	}
}

void AKartPawn::UpdateSpeed(float DeltaTime)
{
	if (BoostTimeRemaining > 0.f)
	{
		CurrentSpeed = BoostSpeed;
		return;
	}
	
	if (!bIsGrounded)
	{
		return;
	}
	
	float TargetSpeed = 0.f;
	float Rate = CoastDeceleration;
	
	if (ThrottleInput > 0.f)
	{
		TargetSpeed = GetCurrentMaxSpeed() * ThrottleInput;
		Rate = CurrentSpeed < 0.f ? BrakeDeceleration : Acceleration;
	}
	
	else if (ThrottleInput < 0.f)
	{
		TargetSpeed = MaxReverseSpeed * ThrottleInput;
		Rate = CurrentSpeed > 0.f ? BrakeDeceleration : Acceleration;
	}
	
	CurrentSpeed = FMath::FInterpConstantTo(CurrentSpeed, TargetSpeed, DeltaTime, Rate);
}

void AKartPawn::UpdateSteering(float DeltaTime)
{
	if (bIsGrounded || FMath::Abs(CurrentSpeed) < MinSpeedToTurn)
	{
		return;
	}
	
	float Steer = SteerInput;
	float Rate = TurnRate;
	
	if (bIsDrifting)
	{
		const float SteerAlongDrift = SteerInput * DriftDirection;
		const float DriftSteer = DriftDirection * FMath:: GetMappedRangeValueClamped(FVector2D(-1.f, 1.f), FVector2D(0.5f, 1.f), SteerAlongDrift);
		
		const float Blend = FMath::SmoothStep(0.f, 1.f, DriftBlend);
		Steer = FMath::Lerp(SteerInput, DriftSteer, Blend);
		Rate *= FMath::Lerp(1.f, DriftTurnMultiplier, Blend);
	}
	
	const float MoveDirection = CurrentSpeed >= 0.f ? 1.f : -1.f;
	
	const float YawDelta = Steer * Rate* MoveDirection* DeltaTime;
	AddActorLocalRotation(FRotator(0.f, YawDelta, 0.f));
}
