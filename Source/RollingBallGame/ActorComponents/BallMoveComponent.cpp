// Fill out your copyright notice in the Description page of Project Settings.

#include "BallMoveComponent.h"
#include "RollingBallGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"

UBallMoveComponent::UBallMoveComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UBallMoveComponent::Move(FVector2d Move) const
{
    if (Move.Length() > 1.0f)
    {
        Move.Normalize();
    }

    const float DeltaTime = GetWorld()->GetDeltaSeconds();
    const FVector3d Forward = Camera->GetForwardVector() * -Move.X * TorqueForce * DeltaTime;
    const FVector3d Right = Camera->GetRightVector() * Move.Y * TorqueForce * DeltaTime;

    Sphere->AddTorqueInDegrees(Forward + Right, NAME_None, true);
}

void UBallMoveComponent::BeginPlay()
{
    Super::BeginPlay();

    const ARollingBallGameCharacter* RollingBall = Cast<ARollingBallGameCharacter>(GetOwner());
    Sphere = RollingBall->GetSphere();
    Camera = RollingBall->GetCamera();

    Sphere->SetPhysicsMaxAngularVelocityInRadians(MaxAngularVelocity);
}

void UBallMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
