// Fill out your copyright notice in the Description page of Project Settings.

#include "LaunchAbilityComponent.h"
#include "RollingBallGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Subsystems/TimeDilationSubsystem.h"

ULaunchAbilityComponent::ULaunchAbilityComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

bool ULaunchAbilityComponent::HasDisabledReason(ELaunchAbilityDisableReasons Reason) const
{
    return EnumHasAnyFlags(DisableReasons, Reason);
}

void ULaunchAbilityComponent::SetDisabledReason(ELaunchAbilityDisableReasons Reason, bool Value)
{
    if (HasDisabledReason(Reason) == Value)
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
        SetRunning(false);
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

    if (AimRequested && !IsDisabled())
    {
        SetRunning(true);
    }
}

void ULaunchAbilityComponent::EndAim()
{
    AimRequested = false;
    ExitTimer = ExitAimDelay;
}

void ULaunchAbilityComponent::Launch()
{
    const FVector DirUp = FVector::UpVector * 0.15f;
    const FVector DirRight = Camera->GetRightVector() * 0.05f;
    const FVector DirForward = Camera->GetForwardVector();
    const FVector DirCombined = DirUp + DirRight + DirForward;

    Sphere->SetPhysicsLinearVelocity(DirCombined * LaunchForce);
    // TODO - uncomment once recharge powerups are implemented
    //SetDisabledReason(ELaunchAbilityDisableReasons::NoCharge, true);

    FTimerDelegate Delegate;
    Delegate.BindUObject(this, &ULaunchAbilityComponent::Recharge);
}

void ULaunchAbilityComponent::Recharge()
{
    SetDisabledReason(ELaunchAbilityDisableReasons::NoCharge, false);
}

void ULaunchAbilityComponent::BeginPlay()
{
    Super::BeginPlay();

    const ARollingBallGameCharacter* RollingBall = Cast<ARollingBallGameCharacter>(GetOwner());
    Camera = RollingBall->GetCamera();
    CameraBoom = RollingBall->GetCameraBoom();
    Sphere = RollingBall->GetSphere();

    InitialOffset = CameraBoom->SocketOffset;
    InitialFov = Camera->FieldOfView;
    InitialCameraLag = CameraBoom->CameraLagSpeed;
    InitialCameraRotationLag = CameraBoom->CameraRotationLagSpeed;

    if (!StartWithCharge)
    {
        // TODO - uncomment once recharge powerups are implemented
        //SetDisabledReason(ELaunchAbilityDisableReasons::NoCharge, true);
    }
}

void ULaunchAbilityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

void ULaunchAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    const float UnscaledDeltaTime = FApp::GetDeltaTime();
    TickLerp(UnscaledDeltaTime);
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
        if (IsRunning)
        {
            if (ExitTimer > 0)
            {
                ExitTimer -= UnscaledDeltaTime;

                if (ExitTimer <= 0.0f)
                {
                    SetRunning(false);
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

void ULaunchAbilityComponent::SetRunning(bool Running)
{
    if (IsRunning == Running)
    {
        return;
    }

    IsRunning = Running;
    AimStateChanged.Broadcast(Running);
    const FName RequesterId = FName("RollingBall");

    if (Running)
    {
        GetWorld()->GetSubsystem<UTimeDilationSubsystem>()->RequestDilation(RequesterId, TimeDilation);
    }
    else
    {
        GetWorld()->GetSubsystem<UTimeDilationSubsystem>()->ClearRequest(RequesterId);
    }
}
