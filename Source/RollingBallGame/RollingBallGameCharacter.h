// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RollingBallGameCharacter.generated.h"

class USphereComponent;
class UCharacterMovementComponent;
class USpringArmComponent;
class UCameraComponent;

enum EJumpChargeAdjustReasons
{
	Jumped,
	GroundedRecharge,
};

UCLASS(Abstract)
class ROLLINGBALLGAME_API ARollingBallGameCharacter : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Abilities");
	float JumpRecoverDelay = 3.0f;

	UPROPERTY(EditAnywhere, Category="Abilities");
	int MaxJumpCharges = 3;

	UPROPERTY(EditAnywhere, Category="Abilities");
	float JumpForce = 10.0f;

	UPROPERTY(EditAnywhere, Category="Abilities");
	float TorqueForce = 200000.0f;
	
	UPROPERTY(EditAnywhere, Category="Abilities");
	float MaxAngularVelocity = 10.0f;
	
	ARollingBallGameCharacter();
	
	virtual void Tick(float DeltaTime) override;
	
	void Move(FVector2d X) const;
	void Look(const double X, const double Y);
	void Jump();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", Meta=(AllowPrivateAccess = "true"))
	USphereComponent* Sphere = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", Meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", Meta=(AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", Meta=(AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera = nullptr;
	
	void HandleGroundedJumpRecharge();
	void AdjustJumpCharges(int Amount, EJumpChargeAdjustReasons Reason);

	float dt = 0;
	float JumpRechargeTimer = 0;
	int JumpCharges = 0;
	bool IsGrounded = false;
};
