// Fill out your copyright notice in the Description page of Project Settings.

#include "BallAudioComponent.h"
#include "BallMoveComponent.h"
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
    RollingLoopAudio = RollingBall->GetRollingLoopAudio();
    RollingLoopAudio->SetSound(RollingSound);
    RollingLoopAudio->Play();

    Sphere->OnComponentHit.AddDynamic(this, &UBallAudioComponent::OnHit);
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

// ReSharper disable once CppMemberFunctionMayBeConst
void UBallAudioComponent::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
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

