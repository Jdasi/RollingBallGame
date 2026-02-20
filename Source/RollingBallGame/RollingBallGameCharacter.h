// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RollingBallGameCharacter.generated.h"

class USphereComponent;
class UCharacterMovementComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS(Abstract)
class ROLLINGBALLGAME_API ARollingBallGameCharacter : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Ability");
	float JumpForce = 10.0f;

	UPROPERTY(EditAnywhere, Category="Ability");
	float TorqueForce = 200000.0f;
	
	UPROPERTY(EditAnywhere, Category="Ability");
	float MaxAngularVelocity = 10.0f;
	
	ARollingBallGameCharacter();
	
	virtual void Tick(float DeltaTime) override;
	
	void Move(FVector2d X) const;
	void Look(const double X, const double Y);
	void Jump() const;

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
	
	float dt = 0;
};
