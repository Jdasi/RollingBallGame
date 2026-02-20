// Fill out your copyright notice in the Description page of Project Settings.

#include "RollingBallGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"

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
	
	// create the camera boom
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

	// create the orbiting camera
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
	
	Sphere->SetPhysicsMaxAngularVelocityInRadians(MaxAngularVelocity);
}

void ARollingBallGameCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	dt = DeltaTime;
}

void ARollingBallGameCharacter::Move(FVector2d Move) const
{
	if (Move.Length() > 1.0f)
	{
		Move.Normalize();
	}
	
	const FVector3d Forward = FollowCamera->GetForwardVector() * -Move.X * TorqueForce * dt;
	const FVector3d Right = FollowCamera->GetRightVector() * Move.Y * TorqueForce * dt;
	
	Sphere->AddTorqueInDegrees(Forward + Right, NAME_None, true);
}

void ARollingBallGameCharacter::Look(const double X, const double Y)
{
	AddControllerYawInput(X);
	AddControllerPitchInput(Y);
}

