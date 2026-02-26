// Copyright Epic Games, Inc. All Rights Reserved.

#include "RollingBallGamePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RollingBallGameCharacter.h"
#include "RollingBallGameHUD.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/KismetSystemLibrary.h"

// seems to be called before BeginPlay (possibly due to GameMode spawning the player which is auto-possessed?)
void ARollingBallGamePlayerController::OnPossess(APawn* APawn)
{
	Super::OnPossess(APawn);

	if (!EnhancedInputComponent)
	{
		EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	}

	if (!RollingBallHUD)
	{
		RollingBallHUD = Cast<ARollingBallGameHUD>(GetHUD());
	}

	RollingBall = Cast<ARollingBallGameCharacter>(APawn);

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

	if (ActionQuit)
	{
		EnhancedInputComponent->BindAction(ActionQuit, ETriggerEvent::Triggered, this, &ARollingBallGamePlayerController::Quit);
	}

	if (ActionReload)
	{
		EnhancedInputComponent->BindAction(ActionReload, ETriggerEvent::Triggered, this, &ARollingBallGamePlayerController::RestartLevel);
	}

	RollingBallHUD->OnPossessRollingBall(RollingBall);
}

void ARollingBallGamePlayerController::OnUnPossess()
{
	EnhancedInputComponent->ClearActionBindings();
	RollingBallHUD->OnUnPossessRollingBall();
	RollingBall = nullptr;

	Super::OnUnPossess();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ARollingBallGamePlayerController::HandleMove(const FInputActionValue& InputActionValue)
{
	const FVector2d Axis = InputActionValue.Get<FVector2D>();
	RollingBall->HandleMoveAction(Axis);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ARollingBallGamePlayerController::HandleLook(const FInputActionValue& InputActionValue)
{
	const FVector2d LookAxisVector = InputActionValue.Get<FVector2D>();
	AddYawInput(LookAxisVector.X * LookSensitivity.X);
	AddPitchInput(LookAxisVector.Y * LookSensitivity.Y);
}

void ARollingBallGamePlayerController::HandleMouseLook(const FInputActionValue& InputActionValue)
{
	HandleLook(InputActionValue);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ARollingBallGamePlayerController::HandleJump(const FInputActionValue& InputActionValue)
{
	RollingBall->HandleJumpAction();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ARollingBallGamePlayerController::StartAim(const FInputActionValue& InputActionValue)
{
	RollingBall->HandleAimStartAction();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ARollingBallGamePlayerController::EndAim(const FInputActionValue& InputActionValue)
{
	RollingBall->HandleAimEndAction();
}

void ARollingBallGamePlayerController::Quit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, false);
}
