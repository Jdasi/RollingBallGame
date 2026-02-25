// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LaunchAbilityComponent.generated.h"

class USphereComponent;
class UCameraComponent;
class USpringArmComponent;

UENUM(BlueprintType, meta=(Bitflags))
enum ELaunchAbilityDisableReasons : uint8
{
    None = 0 UMETA(Hidden),
    NoCharge = 1 << 0,
};
ENUM_CLASS_FLAGS(ELaunchAbilityDisableReasons)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROLLINGBALLGAME_API ULaunchAbilityComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category="Settings")
    float LaunchForce = 3500.0f;

    UPROPERTY(EditAnywhere, Category="Settings")
    float LaunchUpFactor = 0.15f;

    UPROPERTY(EditAnywhere, Category="Settings")
    float LaunchRightFactor = 0.05f;

    UPROPERTY(EditAnywhere, Category="Settings")
    float TimeDilation = 0.1f;

    UPROPERTY(EditAnywhere, Category="Settings")
    float ExitAimDelay = 0.1f;

    UPROPERTY(EditAnywhere, Category="Settings")
    bool StartWithCharge = false;

    UPROPERTY(EditAnywhere, Category="Lerp Targets")
    FVector OffsetLerpTarget = FVector(75.0f, 125.0f, 25.0f);

    UPROPERTY(EditAnywhere, Category="Lerp Targets")
    float FovLerpTarget = 50.0f;

    UPROPERTY(EditAnywhere, Category="Lerp Targets")
    float CameraLagLerpTarget = 100.0f;

    UPROPERTY(EditAnywhere, Category="Lerp Speeds")
    float OffsetLerpSpeed = 10.0f;

    UPROPERTY(EditAnywhere, Category="Lerp Speeds")
    float FovLerpSpeed = 7.0f;

    UPROPERTY(EditAnywhere, Category="Lerp Speeds")
    float CameraLagLerpSpeed = 5.0f;

    UPROPERTY(EditAnywhere, Category="Lerp Speeds")
    float CameraRotationLagLerpSpeed = 5.0f;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLaunchAvailabilityChanged,
        bool, Available);
    UPROPERTY(BlueprintReadOnly, Category="Events")
    FLaunchAvailabilityChanged LaunchAvailabilityChanged;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRollingBallAimStateChanged,
        bool, Started);
    UPROPERTY(BlueprintReadOnly, Category="Events")
    FRollingBallAimStateChanged AimStateChanged;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRollingBallLaunched);
    UPROPERTY(BlueprintReadOnly, Category="Events")
    FRollingBallLaunched Launched;

    UFUNCTION(BlueprintCallable, Category = "Components|RollingBall")
    bool Recharge();

    ULaunchAbilityComponent();

    FORCEINLINE bool IsDisabled() const { return DisableReasons != ELaunchAbilityDisableReasons::None; }
    FORCEINLINE bool ShouldConsumeJumpAction() const { return IsRunning; }
    void StartAim();
    void EndAim();
    void Launch();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    UCameraComponent* Camera = nullptr;

    UPROPERTY()
    USpringArmComponent* CameraBoom = nullptr;

    UPROPERTY()
    USphereComponent* Sphere = nullptr;

    FVector InitialOffset = FVector::ZeroVector;
    ELaunchAbilityDisableReasons DisableReasons = ELaunchAbilityDisableReasons::None;
    float InitialFov = 0.0f;
    float InitialCameraLag = 0.0f;
    float InitialCameraRotationLag = 0.0f;
    float ExitTimer = 0.0f;
    bool IsRunning = false;
    bool AimRequested = false;

    void TickLerp(float UnscaledDeltaTime);
    void SetRunning(bool Running);
    bool HasDisabledReason(ELaunchAbilityDisableReasons Reason) const;
    void SetDisabledReason(ELaunchAbilityDisableReasons Reason, bool Value);
};
