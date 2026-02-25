// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BallJumpComponent.generated.h"

class ARollingBallGamePlayerState;
class UBallMoveComponent;

UENUM(BlueprintType)
enum EJumpChargeAdjustReasons : uint8
{
    Generic,
    OnPossess,
    Jumped,
    Launched,
    GroundedRecharge,
    MaxChargesChanged,
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROLLINGBALLGAME_API UBallJumpComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category="Settings")
    float JumpCooldown = 0.25f;

    UPROPERTY(EditAnywhere, Category="Settings")
    float JumpRechargeRate = 0.5f;

    UPROPERTY(EditAnywhere, Category="Settings")
    float JumpForce = 800.0f;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRollingBallJumpChargesChanged,
        int32, OldValue,
        int32, NewValue,
        EJumpChargeAdjustReasons, Reason);
    UPROPERTY(BlueprintReadOnly, Category="Events")
    FRollingBallJumpChargesChanged JumpChargesChanged;

    UFUNCTION(BlueprintCallable, Category = "Components|RollingBall")
    FORCEINLINE int GetJumpCharges() const { return JumpCharges; }

    UFUNCTION(BlueprintCallable, Category = "Components|RollingBall")
    bool AdjustJumpCharges(int Amount, EJumpChargeAdjustReasons Reason);

    UBallJumpComponent();

    void PossessedBy(AController* NewController);
    void Jump();
    void ClearJumpCooldown();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    ARollingBallGamePlayerState* PlayerState;

    UPROPERTY()
    UBallMoveComponent* MoveComponent;

    FTimerHandle JumpCooldownHandle;
    float JumpRechargeTimer = 0;
    int JumpCharges = 0;

    void HandleGroundedJumpRecharge();
};
