// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LaunchAbilityComponent.generated.h"

enum EJumpChargeAdjustReasons : uint8;
class UBallJumpComponent;
class USphereComponent;
class UCameraComponent;
class USpringArmComponent;

UENUM(BlueprintType, meta=(Bitflags))
enum ELaunchAbilityDisableReasons : uint8
{
    None = 0 UMETA(Hidden),
    NearGeometry = 1 << 0,
    RecentLaunch = 1 << 1,
    NoJumpCharges = 1 << 2,
};
ENUM_CLASS_FLAGS(ELaunchAbilityDisableReasons)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROLLINGBALLGAME_API ULaunchAbilityComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category="Settings")
    float LaunchForce = 3000.0f;

    UPROPERTY(EditAnywhere, Category="Settings")
    float LaunchCooldown = 5.0f;

    UPROPERTY(EditAnywhere, Category="Settings")
    float RequiredGeometryDistance = 200.0f;

    UPROPERTY(EditAnywhere, Category="Settings")
    float TimeDilation = 0.4f;

    UPROPERTY(EditAnywhere, Category="Settings")
    float ExitAimDelay = 0.5f;

    UPROPERTY(EditAnywhere, Category="Lerp Targets")
    FVector OffsetLerpTarget = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, Category="Lerp Targets")
    float FovLerpTarget = 50.0f;

    UPROPERTY(EditAnywhere, Category="Lerp Targets")
    float CameraLagLerpTarget = 1.0f;

    UPROPERTY(EditAnywhere, Category="Lerp Speeds")
    float OffsetLerpSpeed = 10.0f;

    UPROPERTY(EditAnywhere, Category="Lerp Speeds")
    float FovLerpSpeed = 10.0f;

    UPROPERTY(EditAnywhere, Category="Lerp Speeds")
    float CameraLagLerpSpeed = 10.0f;

    UPROPERTY(EditAnywhere, Category="Lerp Speeds")
    float CameraRotationLagLerpSpeed = 10.0f;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRollingBallAimStateChanged,
        bool, Started);
    UPROPERTY(BlueprintReadOnly, Category="Events")
    FRollingBallAimStateChanged AimStateChanged;

    ULaunchAbilityComponent();

    FORCEINLINE bool IsDisabled() const { return DisableReasons != ELaunchAbilityDisableReasons::None; }
    FORCEINLINE bool ShouldConsumeJump() const { return IsRunning; }
    bool HasDisabledFlag(ELaunchAbilityDisableReasons Reason) const;
    void SetDisabledReason(ELaunchAbilityDisableReasons Reason, bool Value);
    void StartAim();
    void EndAim();
    void Launch();
    void ClearLaunchCooldown();

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

    UPROPERTY()
    UBallJumpComponent* JumpComponent = nullptr;

    FVector InitialOffset = FVector::ZeroVector;
    ELaunchAbilityDisableReasons DisableReasons = ELaunchAbilityDisableReasons::None;
    FTimerHandle LaunchCooldownHandle;
    float InitialFov = 0.0f;
    float InitialCameraLag = 0.0f;
    float InitialCameraRotationLag = 0.0f;
    float GeometryCheckTimer;
    float ExitTimer = 0.0f;
    bool IsRunning = false;
    bool AimRequested = false;

    UFUNCTION()
    void OnJumpChargesChanged(int PrevValue, int NewValue, EJumpChargeAdjustReasons Reason);

    void SetRunning(bool Running);
    void TickGeometryCheck(float UnscaledDeltaTime);
    void TickLerp(float UnscaledDeltaTime);
    void PerformGeometryCheck();
};
