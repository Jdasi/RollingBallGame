// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RollingBallGameCharacter.h"
#include "Components/ActorComponent.h"
#include "BallMoveComponent.generated.h"

class USphereComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROLLINGBALLGAME_API UBallMoveComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category="Settings")
    float TorqueForce = 8000.0f;

    UPROPERTY(EditAnywhere, Category="Settings")
    float AirborneForce = 750.0f;

    UPROPERTY(EditAnywhere, Category="Settings")
    float MaxAngularVelocity = 30.0f;

    UPROPERTY(EditAnywhere, Category="Settings")
    float GroundedCheckLength = 20.0f;

    UPROPERTY(EditAnywhere, Category="Settings")
    float GroundedCheckRadiusFactor = 0.8f;

    UBallMoveComponent();

    FORCEINLINE bool GetIsGrounded() const { return IsGrounded; }
    void Move(FVector2d Move) const;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    AController* Controller = nullptr;

    UPROPERTY()
    USphereComponent* Sphere = nullptr;

    float GeometryCheckTimer = 0.0f;
    bool IsGrounded = false;

    void TickGeometryCheck(float UnscaledDeltaTime);
    void PerformGeometryCheck();
};
