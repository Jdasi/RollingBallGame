// Fill out your copyright notice in the Description page of Project Settings.

#include "LaunchAbilityComponent.h"
#include "BallJumpComponent.h"
#include "RollingBallGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
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

void ULaunchAbilityComponent::SetDisabledReason(ELaunchAbilityDisableReasons Reason, bool Value)
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
    JumpComponent->AdjustJumpCharges(-1, EJumpChargeAdjustReasons::Launched);
    Sphere->SetPhysicsLinearVelocity(Camera->GetForwardVector() * LaunchForce);
    SetDisabledReason(ELaunchAbilityDisableReasons::RecentLaunch, true);

    FTimerDelegate Delegate;
    Delegate.BindUObject(this, &ULaunchAbilityComponent::ClearLaunchCooldown);
    GetWorld()->GetTimerManager().SetTimer(LaunchCooldownHandle, Delegate, LaunchCooldown, false);
}

void ULaunchAbilityComponent::ClearLaunchCooldown()
{
    GetWorld()->GetTimerManager().ClearTimer(LaunchCooldownHandle);
    SetDisabledReason(ELaunchAbilityDisableReasons::RecentLaunch, false);
}

void ULaunchAbilityComponent::BeginPlay()
{
    Super::BeginPlay();

    const ARollingBallGameCharacter* RollingBall = Cast<ARollingBallGameCharacter>(GetOwner());
    Camera = RollingBall->GetCamera();
    CameraBoom = RollingBall->GetCameraBoom();
    Sphere = RollingBall->GetSphere();
    JumpComponent = RollingBall->JumpComponent;
    JumpComponent->JumpChargesChanged.AddDynamic(this, &ULaunchAbilityComponent::OnJumpChargesChanged);

    InitialOffset = CameraBoom->SocketOffset;
    InitialFov = Camera->FieldOfView;
    InitialCameraLag = CameraBoom->CameraLagSpeed;
    InitialCameraRotationLag = CameraBoom->CameraRotationLagSpeed;

    OnJumpChargesChanged(0, RollingBall->JumpComponent->GetJumpCharges(), EJumpChargeAdjustReasons::BeginPlay);
}

void ULaunchAbilityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    JumpComponent->JumpChargesChanged.RemoveDynamic(this, &ULaunchAbilityComponent::OnJumpChargesChanged);
    ClearLaunchCooldown();
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

void ULaunchAbilityComponent::OnJumpChargesChanged(int PrevValue, int NewValue, EJumpChargeAdjustReasons Reason)
{
    SetDisabledReason(ELaunchAbilityDisableReasons::NoJumpCharges, NewValue == 0);

    if (NewValue > PrevValue)
    {
        ClearLaunchCooldown();
    }
}

void ULaunchAbilityComponent::SetRunning(bool Running)
{
    if (IsRunning == Running)
    {
        return;
    }

    IsRunning = Running;
    const FName RequesterId = FName("RollingBall");

    // TODO - notify (UI / audio)

    if (Running)
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

    SetDisabledReason(ELaunchAbilityDisableReasons::NearGeometry, IsNearGeometry);
}
