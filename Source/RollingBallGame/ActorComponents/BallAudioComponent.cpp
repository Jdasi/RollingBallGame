// Fill out your copyright notice in the Description page of Project Settings.

#include "BallAudioComponent.h"

#include "BallJumpComponent.h"
#include "BallMoveComponent.h"
#include "LaunchAbilityComponent.h"
#include "RollingBallGameCharacter.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

UBallAudioComponent::UBallAudioComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UBallAudioComponent::BeginPlay()
{
    Super::BeginPlay();

    const ARollingBallGameCharacter* RollingBall = Cast<ARollingBallGameCharacter>(GetOwner());
    Sphere = RollingBall->GetSphere();
    MoveComponent = RollingBall->MoveComponent;
    JumpComponent = RollingBall->JumpComponent;
    LaunchAbilityComponent = RollingBall->LaunchAbilityComponent;
    RollingLoopAudio = RollingBall->GetRollingLoopAudio();

    Sphere->OnComponentHit.AddDynamic(this, &UBallAudioComponent::OnBallImpact);
    RollingLoopAudio->SetSound(RollingSound);
    RollingLoopAudio->Play();

    JumpComponent->JumpChargesChanged.AddDynamic(this, &UBallAudioComponent::OnBallJumpChargesChanged);
    LaunchAbilityComponent->AimStateChanged.AddDynamic(this, &UBallAudioComponent::OnBallAimStateChanged);
    LaunchAbilityComponent->Launched.AddDynamic(this, &UBallAudioComponent::OnBallLaunched);
}

void UBallAudioComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    UpdateRollingAudio();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UBallAudioComponent::OnBallImpact(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    const float Strength = NormalImpulse.Length();

    // basic contact with floor generates high values
    if (Strength <= MinImpactThreshold)
    {
        return;
    }

    const float Volume = FMath::Clamp((Strength - MinImpactThreshold) / MaxImpactThreshold, 0.0f, MaxImpactVolume);
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Sphere->GetComponentLocation(), Volume, 1.0f);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UBallAudioComponent::OnBallJumpChargesChanged(int32 OldValue, int32 NewValue, EJumpChargeAdjustReasons Reason)
{
    switch (Reason)
    {
        case EJumpChargeAdjustReasons::Jumped: UGameplayStatics::PlaySoundAtLocation(GetWorld(), JumpSound, Sphere->GetComponentLocation()); break;
        case EJumpChargeAdjustReasons::GroundedRecharge: UGameplayStatics::PlaySoundAtLocation(GetWorld(), JumpRechargeSound, Sphere->GetComponentLocation()); break;

        default: break;
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UBallAudioComponent::OnBallAimStateChanged(bool Started)
{
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), Started ? AimStartSound : AimEndSound, Sphere->GetComponentLocation());
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UBallAudioComponent::OnBallLaunched()
{
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), LaunchSound, Sphere->GetComponentLocation());
}

void UBallAudioComponent::UpdateRollingAudio() const
{
    const FVector Velocity = MoveComponent->GetOwner()->GetVelocity();
    const float TimeDilation = GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation();
    const float Speed = MoveComponent->GetIsTouchingGeometry() ? Velocity.Length() * TimeDilation : 0;
    RollingLoopAudio->SetFloatParameter(TEXT("BallSpeed"), Speed);
}
