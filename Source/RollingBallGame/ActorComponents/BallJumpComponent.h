// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BallJumpComponent.generated.h"

UENUM()
enum EJumpChargeAdjustReasons
{
    Jumped,
    GroundedRecharge,
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROLLINGBALLGAME_API UBallJumpComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category="Settings")
    float JumpRecoverDelay = 3.0f;

    UPROPERTY(EditAnywhere, Category="Settings")
    int MaxJumpCharges = 3;

    UPROPERTY(EditAnywhere, Category="Settings")
    float JumpForce = 1000.0f;

    DECLARE_DYNAMIC_DELEGATE_TwoParams(FRollingBallJumpChargesChanged,
        int32, OldValue,
        int32, NewValue);
    UPROPERTY(BlueprintReadOnly, Category="Events")
    FRollingBallJumpChargesChanged JumpChargesChanged;

    UBallJumpComponent();

    void Jump();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Components|JumpingBall")
    void AdjustJumpCharges(int Amount, EJumpChargeAdjustReasons Reason);

private:
    void HandleGroundedJumpRecharge();

    float JumpRechargeTimer = 0;
    int JumpCharges = 0;
    bool IsGrounded = false;
};
