// Fill out your copyright notice in the Description page of Project Settings.

#include "BallAudioComponent.h"
#include "BallMoveComponent.h"
#include "RollingBallGameCharacter.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

UBallAudioComponent::UBallAudioComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UBallAudioComponent::BeginPlay()
{
    Super::BeginPlay();

    const ARollingBallGameCharacter* RollingBall = Cast<ARollingBallGameCharacter>(GetOwner());
    MoveComponent = RollingBall->MoveComponent;
    RollingLoopAudio = RollingBall->GetRollingLoopAudio();
    RollingLoopAudio->SetSound(RollingSound);
    RollingLoopAudio->Play();
}

void UBallAudioComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    UpdateRollingAudio();
}

void UBallAudioComponent::UpdateRollingAudio() const
{
    const FVector Velocity = MoveComponent->GetOwner()->GetVelocity();
    const float Speed = MoveComponent->GetIsTouchingGeometry() ? Velocity.Length() : 0;

    RollingLoopAudio->SetFloatParameter(TEXT("BallSpeed"), Speed);
}

