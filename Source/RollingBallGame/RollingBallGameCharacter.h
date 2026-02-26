// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RollingBallGameCharacter.generated.h"

class UBallVisualsComponent;
class UBallAudioComponent;
class UBallMoveComponent;
class UBallJumpComponent;
class UCameraComponent;
class UCharacterMovementComponent;
class ULaunchAbilityComponent;
class USphereComponent;
class USpringArmComponent;

UCLASS(Abstract)
class ROLLINGBALLGAME_API ARollingBallGameCharacter : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RollingBall|Components")
	UBallMoveComponent* MoveComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RollingBall|Components")
	UBallJumpComponent* JumpComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RollingBall|Components")
	ULaunchAbilityComponent* LaunchAbilityComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RollingBall|Components")
	UBallAudioComponent* AudioComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="RollingBall|Components")
	UBallVisualsComponent* VisualsComponent = nullptr;

	UFUNCTION(BlueprintCallable, Category="RollingBall|Helpers")
	void TeleportBall(AActor* TeleportTarget);

	ARollingBallGameCharacter();

	FORCEINLINE USphereComponent* GetSphere() const { return Sphere; }
	FORCEINLINE UCameraComponent* GetCamera() const { return FollowCamera; }
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }
	FORCEINLINE UAudioComponent* GetRollingLoopAudio() const { return RollingLoopAudio; }

	void HandleMoveAction(const FVector2d& Axis) const;
	void HandleJumpAction() const;
	void HandleAimStartAction() const;
	void HandleAimEndAction() const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", Meta=(AllowPrivateAccess = "true"))
	USphereComponent* Sphere = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", Meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", Meta=(AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", Meta=(AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", Meta=(AllowPrivateAccess = "true"))
	UAudioComponent* RollingLoopAudio = nullptr;

	virtual void PossessedBy(AController* NewController) override;
};
