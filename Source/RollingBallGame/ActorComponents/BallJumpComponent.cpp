// Fill out your copyright notice in the Description page of Project Settings.

#include "BallJumpComponent.h"
#include "BallMoveComponent.h"
#include "RollingBallGameCharacter.h"
#include "Components/SphereComponent.h"

UBallJumpComponent::UBallJumpComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UBallJumpComponent::Jump()
{
    if (JumpCharges <= 0)
    {
        return;
    }

    if (JumpCooldownHandle.IsValid())
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

    FTimerDelegate Delegate;
    Delegate.BindUObject(this, &UBallJumpComponent::ClearJumpCooldown);
    GetWorld()->GetTimerManager().SetTimer(JumpCooldownHandle, Delegate, JumpCooldown, false);
}

void UBallJumpComponent::ClearJumpCooldown()
{
    GetWorld()->GetTimerManager().ClearTimer(JumpCooldownHandle);
}

void UBallJumpComponent::BeginPlay()
{
    Super::BeginPlay();

    const ARollingBallGameCharacter* RollingBall = Cast<ARollingBallGameCharacter>(GetOwner());
    MoveComponent = RollingBall->MoveComponent;

    MaxJumpCharges = FMath::Clamp(MaxJumpCharges, 0, MAX_JUMP_CHARGES);
    AdjustJumpCharges(MaxJumpCharges, EJumpChargeAdjustReasons::BeginPlay);
}

void UBallJumpComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    ClearJumpCooldown();
}

void UBallJumpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    HandleGroundedJumpRecharge();
}

void UBallJumpComponent::HandleGroundedJumpRecharge()
{
    if (JumpCharges >= MaxJumpCharges)
    {
        return;
    }

    if (!MoveComponent->GetIsGrounded())
    {
        JumpRechargeTimer = JumpRechargeRate;
        return;
    }

    JumpRechargeTimer -= GetWorld()->GetDeltaSeconds();

    if (JumpRechargeTimer > 0.0f)
    {
        return;
    }

    AdjustJumpCharges(1, EJumpChargeAdjustReasons::GroundedRecharge);
}

bool UBallJumpComponent::AdjustJumpCharges(const int Amount, const EJumpChargeAdjustReasons Reason)
{
    const int PrevCharges = JumpCharges;
    JumpCharges = FMath::Clamp(JumpCharges + Amount, 0, MaxJumpCharges);

    if (JumpCharges == PrevCharges)
    {
        return false;
    }

    JumpRechargeTimer = JumpRechargeRate;
    JumpChargesChanged.Broadcast(PrevCharges, JumpCharges, Reason);

    return true;
}

bool UBallJumpComponent::AdjustMaxJumpCharges(int Amount)
{
    const int PrevMaxJumpCharges = MaxJumpCharges;
    MaxJumpCharges = FMath::Clamp(MaxJumpCharges + Amount, 0, MAX_JUMP_CHARGES);

    if (MaxJumpCharges == PrevMaxJumpCharges)
    {
        return false;
    }

    const int Diff = MaxJumpCharges - PrevMaxJumpCharges;
    AdjustJumpCharges(Diff, EJumpChargeAdjustReasons::MaxChargesChanged);

    if (MaxJumpCharges > PrevMaxJumpCharges)
    {
        ClearJumpCooldown();
    }

    return true;
}
