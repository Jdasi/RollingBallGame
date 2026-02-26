// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "RollingBallGamePlayerController.generated.h"

class ARollingBallGameHUD;
class ARollingBallGameCharacter;
class UEnhancedInputComponent;
class UInputAction;
class UInputMappingContext;

UCLASS(abstract)
class ARollingBallGamePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* APawn) override;
	virtual void OnUnPossess() override;

private:
	UPROPERTY()
	ARollingBallGameHUD* RollingBallHUD = nullptr;

	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent = nullptr;

	UPROPERTY()
	ARollingBallGameCharacter* RollingBall = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess = "true"))
	FVector2D LookSensitivity = FVector2D(1.f, 1.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* InputMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* ActionJump = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* ActionMove = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* ActionLook = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* ActionMouseLook = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* ActionAim = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* ActionQuit = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* ActionReload = nullptr;

	void HandleMove(const FInputActionValue& InputActionValue);
	void HandleLook(const FInputActionValue& InputActionValue);
	void HandleMouseLook(const FInputActionValue& InputActionValue);
	void HandleJump(const FInputActionValue& InputActionValue);
	void StartAim(const FInputActionValue& InputActionValue);
	void EndAim(const FInputActionValue& InputActionValue);
	void Quit();
};
