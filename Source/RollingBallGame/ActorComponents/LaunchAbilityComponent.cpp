// Fill out your copyright notice in the Description page of Project Settings.

#include "LaunchAbilityComponent.h"
#include "RollingBallGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Subsystems/TimeDilationSubsystem.h"

ULaunchAbilityComponent::ULaunchAbilityComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

bool ULaunchAbilityComponent::HasDisabledFlag(ELaunchAbilityDisableReasons Reason) const
{
    return EnumHasAnyFlags(DisableReasons, Reason);
}

void ULaunchAbilityComponent::SetDisabledFlag(ELaunchAbilityDisableReasons Reason, bool Value)
{
    if (HasDisabledFlag(Reason) == Value)
    {
        return;
    }

    bool PrevDisabled = DisableReasons != ELaunchAbilityDisableReasons::None;

    if (Value)
    {
        DisableReasons |= Reason;
    }
    else
    {
        DisableReasons &= ~Reason;
    }

    const bool CurrDisabled = DisableReasons != ELaunchAbilityDisableReasons::None;

    if (CurrDisabled == PrevDisabled)
    {
        return;
    }

    if (CurrDisabled)
    {
        SetPrimed(false);
    }
    else
    {
        if (AimRequested)
        {
            StartAim();
        }
    }
}

void ULaunchAbilityComponent::StartAim()
{
    AimRequested = true;

    if (!IsDisabled() && AimRequested && !IsPrimed)
    {
        SetPrimed(true);
    }
}

void ULaunchAbilityComponent::EndAim()
{
    AimRequested = false;
    ExitTimer = ExitAimDelay;
}

void ULaunchAbilityComponent::BeginPlay()
{
    Super::BeginPlay();

    const ARollingBallGameCharacter* RollingBall = Cast<ARollingBallGameCharacter>(GetOwner());
    Camera = RollingBall->GetCamera();
    CameraBoom = RollingBall->GetCameraBoom();

    InitialOffset = CameraBoom->SocketOffset;
    InitialFov = Camera->FieldOfView;
    InitialCameraLag = CameraBoom->CameraLagSpeed;
    InitialCameraRotationLag = CameraBoom->CameraRotationLagSpeed;
}

void ULaunchAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    const float UnscaledDeltaTime = FApp::GetDeltaTime();
    TickGeometryCheck(UnscaledDeltaTime);
    TickLerp(UnscaledDeltaTime);
}

void ULaunchAbilityComponent::TickGeometryCheck(const float UnscaledDeltaTime)
{
    GeometryCheckTimer -= UnscaledDeltaTime;

    if (GeometryCheckTimer > 0)
    {
        return;
    }

    GeometryCheckTimer = 0.1f;
    PerformGeometryCheck();
}

void ULaunchAbilityComponent::TickLerp(const float UnscaledDeltaTime)
{
    if (!IsDisabled() && AimRequested)
    {
        CameraBoom->SocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, OffsetLerpTarget, UnscaledDeltaTime, OffsetLerpSpeed);
        CameraBoom->CameraLagSpeed = FMath::FInterpTo(CameraBoom->CameraLagSpeed, CameraLagLerpTarget, UnscaledDeltaTime, CameraLagLerpSpeed);
        CameraBoom->CameraRotationLagSpeed  = FMath::FInterpTo(CameraBoom->CameraRotationLagSpeed, CameraLagLerpTarget, UnscaledDeltaTime, CameraLagLerpSpeed);
        Camera->FieldOfView = FMath::FInterpTo(Camera->FieldOfView, FovLerpTarget, UnscaledDeltaTime, FovLerpSpeed);
    }
    else
    {
        if (IsPrimed)
        {
            if (ExitTimer > 0)
            {
                ExitTimer -= UnscaledDeltaTime;

                if (ExitTimer <= 0.0f)
                {
                    SetPrimed(false);
                }
                else
                {
                    return;
                }
            }
        }

        CameraBoom->SocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, InitialOffset, UnscaledDeltaTime, OffsetLerpSpeed);
        CameraBoom->CameraLagSpeed = FMath::FInterpTo(CameraBoom->CameraLagSpeed, InitialCameraLag, UnscaledDeltaTime, CameraLagLerpSpeed);
        CameraBoom->CameraRotationLagSpeed  = FMath::FInterpTo(CameraBoom->CameraRotationLagSpeed, InitialCameraRotationLag, UnscaledDeltaTime, CameraLagLerpSpeed);
        Camera->FieldOfView = FMath::FInterpTo(Camera->FieldOfView, InitialFov, UnscaledDeltaTime, FovLerpSpeed);
    }
}

void ULaunchAbilityComponent::SetPrimed(bool Primed)
{
    if (IsPrimed == Primed)
    {
        return;
    }

    IsPrimed = Primed;
    const FName RequesterId = FName("RollingBall");

    // TODO - notify (UI / audio)

    if (Primed)
    {
        GetWorld()->GetSubsystem<UTimeDilationSubsystem>()->RequestDilation(RequesterId, TimeDilation);
    }
    else
    {
        GetWorld()->GetSubsystem<UTimeDilationSubsystem>()->ClearRequest(RequesterId);
    }
}

void ULaunchAbilityComponent::PerformGeometryCheck()
{
    FCollisionObjectQueryParams ObjectParams;
    ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetOwner());

    const FVector ActorLocation = GetOwner()->GetActorLocation();
    FHitResult Hit;

    const bool IsNearGeometry = GetWorld()->SweepSingleByObjectType(
        Hit,
        ActorLocation,
        ActorLocation,
        FQuat::Identity,
        ObjectParams,
        FCollisionShape::MakeSphere(RequiredGeometryDistance),
        Params);

    SetDisabledFlag(ELaunchAbilityDisableReasons::NearGeometry, IsNearGeometry);
}
