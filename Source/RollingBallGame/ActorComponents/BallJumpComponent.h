// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BallJumpComponent.generated.h"

UENUM(BlueprintType)
enum EJumpChargeAdjustReasons : uint8
{
    BeginPlay,
    Jumped,
    Launched,
    GroundedRecharge,
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROLLINGBALLGAME_API UBallJumpComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    const int MAX_JUMP_CHARGES = 3;

    UPROPERTY(EditAnywhere, Category="Settings")
    float JumpCooldown = 0.5f;

    UPROPERTY(EditAnywhere, Category="Settings")
    float JumpRechargeRate = 3.0f;

    UPROPERTY(EditAnywhere, Category="Settings", Meta=(Tooltip="Range 0-3", ClampMin="0", ClampMax="3"))
    int MaxJumpCharges = 3;

    UPROPERTY(EditAnywhere, Category="Settings")
    float JumpForce = 1000.0f;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRollingBallJumpChargesChanged,
        int32, OldValue,
        int32, NewValue,
        EJumpChargeAdjustReasons, Reason);
    UPROPERTY(BlueprintReadOnly, Category="Events")
    FRollingBallJumpChargesChanged JumpChargesChanged;

    UFUNCTION(BlueprintCallable, Category = "Components|RollingBall")
    FORCEINLINE int GetJumpCharges() const { return JumpCharges; }

    UFUNCTION(BlueprintCallable, Category = "Components|RollingBall")
    FORCEINLINE bool HasJumpCharges() const { return JumpCharges > 0; }

    UFUNCTION(BlueprintCallable, Category = "Components|RollingBall")
    void AdjustJumpCharges(int Amount, EJumpChargeAdjustReasons Reason);

    UBallJumpComponent();

    void Jump();
    void ClearJumpCooldown();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    FTimerHandle JumpCooldownHandle;
    float JumpRechargeTimer = 0;
    int JumpCharges = 0;
    bool IsGrounded = false;

    void HandleGroundedJumpRecharge();

};
