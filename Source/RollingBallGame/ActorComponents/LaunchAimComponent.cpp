// Fill out your copyright notice in the Description page of Project Settings.

#include "LaunchAimComponent.h"
#include "RollingBallGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Subsystems/TimeDilationSubsystem.h"

ULaunchAimComponent::ULaunchAimComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void ULaunchAimComponent::StartAim()
{
    if (!AimRequested)
    {
        SetTimeDilationActive(true);
    }

    AimRequested = true;
    AimActive = true;
}

void ULaunchAimComponent::EndAim()
{
    AimRequested = false;
    ExitTimer = ExitAimDelay;
}

void ULaunchAimComponent::BeginPlay()
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

void ULaunchAimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // time dilation shouldn't affect camera lerp
    DeltaTime = FApp::GetDeltaTime();

    if (AimRequested)
    {
        CameraBoom->SocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, OffsetLerpTarget, DeltaTime, OffsetLerpSpeed);
        CameraBoom->CameraLagSpeed = FMath::FInterpTo(CameraBoom->CameraLagSpeed, CameraLagLerpTarget, DeltaTime, CameraLagLerpSpeed);
        CameraBoom->CameraRotationLagSpeed  = FMath::FInterpTo(CameraBoom->CameraRotationLagSpeed, CameraLagLerpTarget, DeltaTime, CameraLagLerpSpeed);
        Camera->FieldOfView = FMath::FInterpTo(Camera->FieldOfView, FovLerpTarget, DeltaTime, FovLerpSpeed);
    }
    else
    {
        if (AimActive)
        {
            if (ExitTimer > 0)
            {
                ExitTimer -= DeltaTime;

                if (ExitTimer <= 0.0f)
                {
                    AimActive = false;
                    SetTimeDilationActive(false);
                }
                else
                {
                    return;
                }
            }
        }

        CameraBoom->SocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, InitialOffset, DeltaTime, OffsetLerpSpeed);
        CameraBoom->CameraLagSpeed = FMath::FInterpTo(CameraBoom->CameraLagSpeed, InitialCameraLag, DeltaTime, CameraLagLerpSpeed);
        CameraBoom->CameraRotationLagSpeed  = FMath::FInterpTo(CameraBoom->CameraRotationLagSpeed, InitialCameraRotationLag, DeltaTime, CameraLagLerpSpeed);
        Camera->FieldOfView = FMath::FInterpTo(Camera->FieldOfView, InitialFov, DeltaTime, FovLerpSpeed);
    }
}

void ULaunchAimComponent::SetTimeDilationActive(bool Active) const
{
    const FName RequesterId = FName("RollingBall");

    if (Active)
    {
        GetWorld()->GetSubsystem<UTimeDilationSubsystem>()->RequestDilation(RequesterId, TimeDilation);
    }
    else
    {
        GetWorld()->GetSubsystem<UTimeDilationSubsystem>()->ClearRequest(RequesterId);
    }
}
