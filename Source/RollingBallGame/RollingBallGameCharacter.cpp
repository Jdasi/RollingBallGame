// Fill out your copyright notice in the Description page of Project Settings.

#include "RollingBallGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Subsystems/TimeDilationSubsystem.h"

ARollingBallGameCharacter::ARollingBallGameCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	if (Sphere)
	{
		SetRootComponent(Sphere);
		Sphere->InitSphereRadius(42.0f);
		Sphere->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
		Sphere->SetShouldUpdatePhysicsVolume(true);
		Sphere->SetCanEverAffectNavigation(false);
		Sphere->SetSimulatePhysics(true);
		Sphere->CanCharacterStepUpOn = ECB_No;
		Sphere->bDynamicObstacle = true;
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (Mesh)
	{
		Mesh->SetupAttachment(Sphere);
		Mesh->SetCollisionProfileName(TEXT("CharacterMesh"));
		Mesh->SetGenerateOverlapEvents(false);
		Mesh->SetCanEverAffectNavigation(false);
		Mesh->bOwnerNoSee = false;
		Mesh->bCastDynamicShadow = true;
		Mesh->bAffectDynamicIndirectLighting = true;
		Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	}

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	if (CameraBoom)
	{
		CameraBoom->SetupAttachment(RootComponent);
		CameraBoom->TargetArmLength = 400.0f;
		CameraBoom->bUsePawnControlRotation = true;
		CameraBoom->bEnableCameraLag = true;
		CameraBoom->CameraLagSpeed = 8.0f;
		CameraBoom->bEnableCameraRotationLag = true;
		CameraBoom->CameraRotationLagSpeed = 4.0f;
	}

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	if (FollowCamera)
	{
		FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
		FollowCamera->bUsePawnControlRotation = false;
	}

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void ARollingBallGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	// has no effect in the Ctor for some reason
	Sphere->SetPhysicsMaxAngularVelocityInRadians(MaxAngularVelocity);

	JumpCharges = MaxJumpCharges;
}

void ARollingBallGameCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	HandleGroundedJumpRecharge();
}

void ARollingBallGameCharacter::Move(FVector2d Move) const
{
	if (Move.Length() > 1.0f)
	{
		Move.Normalize();
	}

	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	const FVector3d Forward = FollowCamera->GetForwardVector() * -Move.X * TorqueForce * DeltaTime;
	const FVector3d Right = FollowCamera->GetRightVector() * Move.Y * TorqueForce * DeltaTime;

	Sphere->AddTorqueInDegrees(Forward + Right, NAME_None, true);
}

void ARollingBallGameCharacter::Look(const double X, const double Y)
{
	AddControllerYawInput(X);
	AddControllerPitchInput(Y);
}

void ARollingBallGameCharacter::Jump()
{
	if (JumpCharges <= 0)
	{
		return;
	}

	const FVector Velocity = Sphere->GetPhysicsLinearVelocity();
	FVector Impulse = FVector(0.0f, 0.0f, 1.0f) * JumpForce;

	if (Velocity.Z < 0)
	{
		// make jump consistent even while falling
		Impulse.Z += -Velocity.Z;
	}

	Sphere->AddImpulse(Impulse, NAME_None, true);
	AdjustJumpCharges(-1, EJumpChargeAdjustReasons::Jumped);
}

void ARollingBallGameCharacter::StartAim() const
{
	GetWorld()->GetSubsystem<UTimeDilationSubsystem>()->RequestDilation("RollingBall", 0.4f);
}

void ARollingBallGameCharacter::EndAim() const
{
	GetWorld()->GetSubsystem<UTimeDilationSubsystem>()->ClearRequest("RollingBall");
}

void ARollingBallGameCharacter::HandleGroundedJumpRecharge()
{
	if (JumpCharges >= MaxJumpCharges)
	{
		return;
	}

	JumpRechargeTimer -= GetWorld()->GetDeltaSeconds();

	if (JumpRechargeTimer > 0.0f)
	{
		return;
	}

	AdjustJumpCharges(1, EJumpChargeAdjustReasons::GroundedRecharge);
}

void ARollingBallGameCharacter::AdjustJumpCharges(int Amount, EJumpChargeAdjustReasons Reason)
{
	const int PrevCharges = JumpCharges;
	JumpCharges = FMath::Clamp(JumpCharges + Amount, 0, MaxJumpCharges);
	JumpRechargeTimer = JumpRecoverDelay;
	JumpChargesChanged.ExecuteIfBound(PrevCharges, JumpCharges);
}
