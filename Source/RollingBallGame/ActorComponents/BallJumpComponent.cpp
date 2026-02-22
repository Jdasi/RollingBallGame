// Fill out your copyright notice in the Description page of Project Settings.

#include "BallJumpComponent.h"
#include "RollingBallGameCharacter.h"
#include "Components/SphereComponent.h"

UBallJumpComponent::UBallJumpComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UBallJumpComponent::BeginPlay()
{
    Super::BeginPlay();
    JumpCharges = MaxJumpCharges;
}

void UBallJumpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    HandleGroundedJumpRecharge();
}

void UBallJumpComponent::Jump()
{
    if (JumpCharges <= 0)
    {
        return;
    }

    USphereComponent* Sphere = Cast<ARollingBallGameCharacter>(GetOwner())->GetSphere();
    const FVector Velocity = Sphere->GetPhysicsLinearVelocity();
    FVector Impulse = FVector(0.0f, 0.0f, 1.0f) * JumpForce;

    if (Velocity.Z < 0)
    {
        // make jump consistent even while falling
        Impulse.Z += -Velocity.Z;
    }

    Sphere->AddImpulse(Impulse, NAME_None, true);
    AdjustJumpCharges(-1, EJumpChargeAdjustReasons::Jumped);
}

void UBallJumpComponent::HandleGroundedJumpRecharge()
{
    if (JumpCharges >= MaxJumpCharges)
    {
        return;
    }

    JumpRechargeTimer -= GetWorld()->GetDeltaSeconds();

    if (JumpRechargeTimer > 0.0f)
    {
        return;
    }

    AdjustJumpCharges(1, EJumpChargeAdjustReasons::GroundedRecharge);
}

void UBallJumpComponent::AdjustJumpCharges(const int Amount, const EJumpChargeAdjustReasons Reason)
{
    const int PrevCharges = JumpCharges;
    JumpCharges = FMath::Clamp(JumpCharges + Amount, 0, MaxJumpCharges);
    JumpRechargeTimer = JumpRecoverDelay;
    JumpChargesChanged.ExecuteIfBound(PrevCharges, JumpCharges);
}
