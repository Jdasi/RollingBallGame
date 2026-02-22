// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LaunchAbilityComponent.generated.h"

class UCameraComponent;
class USpringArmComponent;

UENUM(BlueprintType, meta=(Bitflags))
enum ELaunchAbilityDisableReasons : uint8
{
    None = 0 UMETA(Hidden),
    NearGeometry = 1 << 0,
};
ENUM_CLASS_FLAGS(ELaunchAbilityDisableReasons)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROLLINGBALLGAME_API ULaunchAbilityComponent : public UActorComponent
{
    GENERATED_BODY()

public:
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

    ULaunchAbilityComponent();

    FORCEINLINE bool IsDisabled() const { return DisableReasons != ELaunchAbilityDisableReasons::None; }
    bool HasDisabledFlag(ELaunchAbilityDisableReasons Reason) const;
    void SetDisabledFlag(ELaunchAbilityDisableReasons Reason, bool Value);
    void StartAim();
    void EndAim();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    UCameraComponent* Camera = nullptr;

    UPROPERTY()
    USpringArmComponent* CameraBoom = nullptr;

    void SetPrimed(bool Primed);
    void TickGeometryCheck(float UnscaledDeltaTime);
    void TickLerp(float UnscaledDeltaTime);
    void PerformGeometryCheck();

    FVector InitialOffset = FVector::ZeroVector;
    ELaunchAbilityDisableReasons DisableReasons = ELaunchAbilityDisableReasons::None;
    float InitialFov = 0.0f;
    float InitialCameraLag = 0.0f;
    float InitialCameraRotationLag = 0.0f;
    float GeometryCheckTimer;
    float ExitTimer = 0.0f;
    bool IsPrimed = false;
    bool AimRequested = false;
};
