// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AMovingPawn::AMovingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMovingPawn::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called every frame
void AMovingPawn::Tick(float DeltaTime)
{
	// Make sure to use TickGroup PostPhysics, so this draws the result at the end of the frame
	Super::Tick(DeltaTime);

	VisualizeMovement();
}

// Called to bind functionality to input
void AMovingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMovingPawn::Look);
	}
}

void AMovingPawn::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMovingPawn::VisualizeMovement() const
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	float HeightOffset = 0.0f;
	const float OffsetPerElement = 10.0f;

	const FVector ShipUp = GetActorUpVector();
	const FVector TopOfCapsule = GetActorLocation() + (GetLocalRole() == ROLE_AutonomousProxy ? (ShipUp * 50.0f) : (ShipUp * 100.0f));

	// Server/Client
	{
		const FColor DebugColor = GetLocalRole() == ROLE_AutonomousProxy ? FColor::Green : GetLocalRole() == ROLE_Authority ? FColor::Red : FColor::Yellow;
		const FVector DebugLocation = TopOfCapsule + ShipUp * HeightOffset;
		FString DebugText = GetLocalRole() == ROLE_AutonomousProxy ? "AutonomousProxy" : GetLocalRole() == ROLE_Authority ? "Authority" : "Simulated";
		DrawDebugString(GetWorld(), DebugLocation, DebugText, nullptr, DebugColor, 0.0f, true);
	}

	// Position
	{
		const FColor DebugColor = FColor::White;
		HeightOffset += OffsetPerElement;
		const FVector DebugLocation = TopOfCapsule + ShipUp * HeightOffset;
		FString DebugText = FString::Printf(TEXT("Position: %s"), *GetActorLocation().ToCompactString());
		DrawDebugString(GetWorld(), DebugLocation, DebugText, nullptr, DebugColor, 0.0f, true);
	}

	// Rotation
	{
		/*const FColor DebugColor = FColor::White;
		HeightOffset += OffsetPerElement;
		const FVector DebugLocation = TopOfCapsule + ShipUp * HeightOffset;

		DrawDebugCoordinateSystem(GetWorld(), DebugLocation + ShipUp * -5.0f, UpdatedComponent->GetComponentRotation(),
			100.0f, false, -1.0f, 0, 2.0f);

		FString DebugText = FString::Printf(TEXT("Rotation: %s"), *UpdatedComponent->GetComponentRotation().ToCompactString());
		DrawDebugString(GetWorld(), DebugLocation, DebugText, nullptr, DebugColor, 0.0f, true);*/
	}

	// Velocity
	{
		const FVector Velocity = GetVelocity();

		const FColor DebugColor = FColor::Green;
		HeightOffset += OffsetPerElement;
		const FVector DebugLocation = TopOfCapsule + ShipUp * HeightOffset;
		DrawDebugDirectionalArrow(GetWorld(), DebugLocation + ShipUp * -5.0f, DebugLocation + ShipUp * -5.0f + Velocity,
			100.0f, DebugColor, false, -1.0f, (uint8)'\000', 10.0f);

		FString DebugText = FString::Printf(TEXT("Velocity: %s (Speed: %.2f)"), *Velocity.ToCompactString(), Velocity.Size());
		DrawDebugString(GetWorld(), DebugLocation, DebugText, nullptr, DebugColor, 0.0f, true);
	}

	// Acceleration
	{
		/*const FColor DebugColor = FColor::Yellow;
		HeightOffset += OffsetPerElement;
		const float MaxAccelerationLineLength = 200.0f;
		const float CurrentAccelAsPercentOfMaxAccel = CurrentMaxAccel > 0.0f ? Acceleration.Size() / CurrentMaxAccel : 1.0f;
		const FVector DebugLocation = TopOfCapsule + ShipUp * HeightOffset;
		DrawDebugDirectionalArrow(GetWorld(), DebugLocation + ShipUp * -5.0f,
			DebugLocation + ShipUp * -5.0f + Acceleration.GetSafeNormal(UE_SMALL_NUMBER) * CurrentAccelAsPercentOfMaxAccel * MaxAccelerationLineLength,
			25.0f, DebugColor, false, -1.0f, (uint8)'\000', 8.0f);

		FString DebugText = FString::Printf(TEXT("Acceleration: %s"), *Acceleration.ToCompactString());
		DrawDebugString(GetWorld(), DebugLocation, DebugText, nullptr, DebugColor, 0.0f, true);*/
	}

#endif // !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
}