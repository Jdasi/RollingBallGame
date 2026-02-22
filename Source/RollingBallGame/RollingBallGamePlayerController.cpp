// Copyright Epic Games, Inc. All Rights Reserved.

#include "RollingBallGamePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "RollingBallGameCharacter.h"

void ARollingBallGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ARollingBallGamePlayerController::OnPossess(APawn* APawn)
{
	Super::OnPossess(APawn);
	
	RollingBall = Cast<ARollingBallGameCharacter>(APawn);
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(InputMappingContext, 0);
	
	if (ActionMove)
	{
		EnhancedInputComponent->BindAction(ActionMove, ETriggerEvent::Triggered, this, &ARollingBallGamePlayerController::HandleMove);
	}
	
	if (ActionLook)
	{
		EnhancedInputComponent->BindAction(ActionLook, ETriggerEvent::Triggered, this, &ARollingBallGamePlayerController::HandleLook);
	}
	
	if (ActionMouseLook)
	{
		EnhancedInputComponent->BindAction(ActionMouseLook, ETriggerEvent::Triggered, this, &ARollingBallGamePlayerController::HandleMouseLook);
	}
	
	if (ActionJump)
	{
		EnhancedInputComponent->BindAction(ActionJump, ETriggerEvent::Started, this, &ARollingBallGamePlayerController::HandleJump);
	}

	if (ActionAim)
	{
		EnhancedInputComponent->BindAction(ActionAim, ETriggerEvent::Started, this, &ARollingBallGamePlayerController::StartAim);
		EnhancedInputComponent->BindAction(ActionAim, ETriggerEvent::Completed, this, &ARollingBallGamePlayerController::EndAim);
	}
}

void ARollingBallGamePlayerController::OnUnPossess()
{
	EnhancedInputComponent->ClearActionBindings();
	Super::OnUnPossess();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ARollingBallGamePlayerController::HandleMove(const FInputActionValue& InputActionValue)
{
	const FVector2d MovementVector = InputActionValue.Get<FVector2D>();
	RollingBall->Move(MovementVector);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ARollingBallGamePlayerController::HandleLook(const FInputActionValue& InputActionValue)
{
	const FVector2d LookAxisVector = InputActionValue.Get<FVector2D>();
	RollingBall->Look(LookAxisVector.X * LookSensitivity.X, LookAxisVector.Y * LookSensitivity.Y);
}

void ARollingBallGamePlayerController::HandleMouseLook(const FInputActionValue& InputActionValue)
{
	HandleLook(InputActionValue);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ARollingBallGamePlayerController::HandleJump(const FInputActionValue& InputActionValue)
{
	RollingBall->Jump();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ARollingBallGamePlayerController::StartAim(const FInputActionValue& InputActionValue)
{
	RollingBall->StartAim();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ARollingBallGamePlayerController::EndAim(const FInputActionValue& InputActionValue)
{
	RollingBall->EndAim();
}

