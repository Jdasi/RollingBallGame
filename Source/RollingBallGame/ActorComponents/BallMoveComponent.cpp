// Fill out your copyright notice in the Description page of Project Settings.

#include "BallMoveComponent.h"
#include "RollingBallGameCharacter.h"
#include "Components/SphereComponent.h"

UBallMoveComponent::UBallMoveComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UBallMoveComponent::PossessedBy(AController* NewController)
{
    Controller = NewController;
}

void UBallMoveComponent::Move(FVector2d Axis) const
{
    if (Axis.Length() > 1.0f)
    {
        Axis.Normalize();
    }

    const FRotator ControlRot = Controller->GetControlRotation();
    const FRotator YawRotation(0.0f, ControlRot.Yaw, 0.0f);
    const FRotationMatrix YawRotationMatrix(YawRotation);

    const FVector FlatForward = YawRotationMatrix.GetUnitAxis(EAxis::X);
    const FVector FlatRight = YawRotationMatrix.GetUnitAxis(EAxis::Y);
    const float TorqueForceToUse = IsGrounded ? TorqueForce : TorqueForce * 0.33f;

    if (!IsGrounded)
    {
        const FVector AirForward = FlatForward * Axis.Y;
        const FVector AirRight = FlatRight * Axis.X;
        const FVector AirMoveDir = AirForward + AirRight;

        FVector FlatVelocity = Sphere->GetPhysicsLinearVelocity();
        double FlatSpeed = FlatVelocity.Length();
        FlatVelocity.Z = 0;
        FlatVelocity.Normalize();

        FVector AirMoveForce = AirMoveDir * AirborneForce;
        const double SpeedFactor = FMath::Clamp(FlatSpeed / 500, 0, 1);
        const double Dot = FVector::DotProduct(AirMoveDir, FlatVelocity);

        if (Dot > 0)
        {
            // air control shouldn't overpower your airborne speed
            AirMoveForce *= 1 - Dot * SpeedFactor;
        }

        Sphere->AddForce(AirMoveForce, NAME_None, true);
    }

    const FVector TorqueForward = FlatForward * -Axis.X;
    const FVector TorqueRight = FlatRight * Axis.Y;
    Sphere->AddTorqueInDegrees((TorqueForward + TorqueRight) * TorqueForceToUse, NAME_None, true);
}

void UBallMoveComponent::BeginPlay()
{
    Super::BeginPlay();

    const ARollingBallGameCharacter* RollingBall = Cast<ARollingBallGameCharacter>(GetOwner());
    Sphere = RollingBall->GetSphere();
    Sphere->SetPhysicsMaxAngularVelocityInRadians(MaxAngularVelocity);
}

void UBallMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    const float UnscaledDeltaTime = FApp::GetDeltaTime();
    TickGeometryCheck(UnscaledDeltaTime);
}

void UBallMoveComponent::TickGeometryCheck(const float UnscaledDeltaTime)
{
    GeometryCheckTimer -= UnscaledDeltaTime;

    if (GeometryCheckTimer > 0)
    {
        return;
    }

    GeometryCheckTimer = 0.1f;
    PerformGeometryCheck();
}

void UBallMoveComponent::PerformGeometryCheck()
{
    FCollisionObjectQueryParams ObjectParams;
    ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetOwner());

    const FVector Start = GetOwner()->GetActorLocation();
    const FVector End = Start - FVector(0.0f, 0.0f, GroundedCheckLength);
    FHitResult Hit;

    IsGrounded = GetWorld()->SweepSingleByObjectType(
        Hit,
        Start,
        End,
        FQuat::Identity,
        ObjectParams,
        FCollisionShape::MakeSphere(Sphere->GetScaledSphereRadius() * GroundedCheckRadiusFactor),
        Params);
}
