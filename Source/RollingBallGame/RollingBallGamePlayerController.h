// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "RollingBallGamePlayerController.generated.h"

class ARollingBallGameHUD;
class ARollingBallGameCharacter;
class UInputAction;
class UInputMappingContext;
class UUserWidget;

UCLASS(abstract)
class ARollingBallGamePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* APawn) override;
	virtual void OnUnPossess() override;
	
	void HandleMove(const FInputActionValue& InputActionValue);
	void HandleLook(const FInputActionValue& InputActionValue);
	void HandleMouseLook(const FInputActionValue& InputActionValue);
	void HandleJump(const FInputActionValue& InputActionValue);
	void StartAim(const FInputActionValue& InputActionValue);
	void EndAim(const FInputActionValue& InputActionValue);
	
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
};
