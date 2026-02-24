// Fill out your copyright notice in the Description page of Project Settings.

#include "RollingBallGameCharacter.h"
#include "ActorComponents/BallJumpComponent.h"
#include "ActorComponents/BallMoveComponent.h"
#include "ActorComponents/LaunchAbilityComponent.h"
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

	MoveComponent = CreateDefaultSubobject<UBallMoveComponent>(TEXT("MoveComponent"));
	JumpComponent = CreateDefaultSubobject<UBallJumpComponent>(TEXT("JumpComponent"));
	LaunchAbilityComponent = CreateDefaultSubobject<ULaunchAbilityComponent>(TEXT("LaunchAbilityComponent"));

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void ARollingBallGameCharacter::HandleMoveAction(const FVector2d& Axis) const
{
	MoveComponent->Move(Axis);
}

void ARollingBallGameCharacter::HandleJumpAction() const
{
	if (LaunchAbilityComponent->ShouldConsumeJumpAction())
	{
		LaunchAbilityComponent->Launch();
	}
	else
	{
		JumpComponent->Jump();
	}
}

void ARollingBallGameCharacter::HandleAimStartAction() const
{
	LaunchAbilityComponent->StartAim();
}

void ARollingBallGameCharacter::HandleAimEndAction() const
{
	LaunchAbilityComponent->EndAim();
}

void ARollingBallGameCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	MoveComponent->PossessedBy(NewController);
}
