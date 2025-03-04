// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "testproj1Pawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/ChildActorComponent.h"

Atestproj1Pawn::Atestproj1Pawn()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Flying/Meshes/UFO.UFO"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create static mesh component
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());	// Set static mesh
	RootComponent = PlaneMesh;

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);	// Attach SpringArm to RootComponent
	SpringArm->TargetArmLength = 160.0f; // The camera follows at this distance behind the character	
	SpringArm->SocketOffset = FVector(0.f,0.f,60.f);
	SpringArm->bEnableCameraLag = false;	// Do not allow camera to lag
	SpringArm->CameraLagSpeed = 15.f;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);	// Attach the camera
	Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

	// Set handling parameters
	Acceleration = 500.f;
	TurnSpeed = 50.f;
	MaxSpeed = 4000.f;
	MinSpeed = 500.f;
	CurrentForwardSpeed = 500.f;
}

void Atestproj1Pawn::Tick(float DeltaSeconds)
{
	// Call any parent class Tick implementation
	Super::Tick(DeltaSeconds);

	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaSeconds, 0.f, 0.f);

	// Move plan forwards (with sweep so we stop when we collide with things)
	AddActorLocalOffset(LocalMove, true);

	// Calculate change in rotation this frame
	FRotator DeltaRotation(0,0,0);
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaSeconds;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaSeconds;
	DeltaRotation.Roll = CurrentRollSpeed * DeltaSeconds;

	// Rotate plane
	AddActorLocalRotation(DeltaRotation);
}

void Atestproj1Pawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Deflect along the surface when we collide.
	FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));

	//충돌이 일어날 시 기체 HP 감소
	//UE_LOG(LogTemp, Warning, TEXT("Pawn Ntfy Hit"));
	//hp--;
}

void Atestproj1Pawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // Check if PlayerInputComponent is valid (not NULL)
	check(PlayerInputComponent);

	// Bind our control axis' to callback functions
	PlayerInputComponent->BindAxis("Thrust", this, &Atestproj1Pawn::ThrustInput);
	PlayerInputComponent->BindAxis("MoveUp", this, &Atestproj1Pawn::MoveUpInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &Atestproj1Pawn::MoveRightInput);
	PlayerInputComponent->BindAxis("MoveForward", this, &Atestproj1Pawn::MoveForwardInput);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &Atestproj1Pawn::ShootInput);
}

void Atestproj1Pawn::ThrustInput(float Val)
{
	
	// Is there any input?
	bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	// If input is not held down, reduce speed
	float CurrentAcc = bHasInput ? (Val * Acceleration) : (-0.5f * Acceleration);
	// Calculate new speed
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
	// Clamp between MinSpeed and MaxSpeed
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
	
}

void Atestproj1Pawn::MoveUpInput(float Val)
{
	// Target pitch speed is based in input
	float TargetPitchSpeed = (Val * TurnSpeed * -1.f);

	// When steering, we decrease pitch slightly
	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

	// Smoothly interpolate to target pitch speed
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

}

void Atestproj1Pawn::MoveRightInput(float Val)
{
	// Target yaw speed is based on input
	float TargetYawSpeed = (Val * TurnSpeed);

	// Smoothly interpolate to target yaw speed
	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

	// Is there any left/right input?
	const bool bIsTurning = FMath::Abs(Val) > 0.2f;

	// If turning, yaw value is used to influence roll
	// If not turning, roll to reverse current roll value.
	float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (GetActorRotation().Roll * -2.f);

	// Smoothly interpolate roll speed
	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void Atestproj1Pawn::MoveForwardInput(float Val)
{
	CurrentForwardSpeed += Val * 100.0f;

	if (CurrentForwardSpeed >= MaxSpeed)
		CurrentForwardSpeed = MaxSpeed;
}

void Atestproj1Pawn::ShootInput()
{
	UE_LOG(LogTemp, Warning, TEXT("shoot"));

	if (BulletClass)
	{
		// 카메라 트랜스폼을 구합니다.
		FVector		CameraLocation;
		FRotator	CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// 액터의 Location, Rotator를 구합니다.
		FVector		actorLocation = GetActorLocation();
		FRotator	actorRotator = GetActorRotation();
				
		// ABullet이 발사될 Location, Rotator를 설정합니다.
		FVector MuzzleLocation = actorLocation + FTransform(actorRotator).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;

		// 조준을 약간 윗쪽으로 올려줍니다.
		MuzzleRotation.Pitch += 45.0f;

		//ABullet을 발사합니다.
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.Instigator = Instigator;

			auto rot = GetActorRotation();

			//스폰
			//ABulletActor* bullet = World->SpawnActor<ABulletActor>(BulletClass, MyActor->GetActorLocation(), MyActor->GetActorRotation(), spawnParams); //pos + FVector(100.0f, 0.0f, 0.0f) FRotator::ZeroRotator
			ABulletActor* bullet = World->SpawnActor<ABulletActor>(BulletClass, MuzzleLocation, MuzzleRotation, spawnParams);

			if (bullet)
			{
				// 발사 방향을 알아냅니다.
				FVector LaunchDirection = rot.Vector();
				bullet->FireInDirection(LaunchDirection);
			}
		}
	}
}
