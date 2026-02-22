// Fill out your copyright notice in the Description page of Project Settings.

#include "LaunchAimComponent.h"
#include "RollingBallGameCharacter.h"
#include "Subsystems/TimeDilationSubsystem.h"

ULaunchAimComponent::ULaunchAimComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void ULaunchAimComponent::StartAim() const
{
    GetWorld()->GetSubsystem<UTimeDilationSubsystem>()->RequestDilation("RollingBall", 0.4f);
}

void ULaunchAimComponent::EndAim() const
{
    GetWorld()->GetSubsystem<UTimeDilationSubsystem>()->ClearRequest("RollingBall");
}

void ULaunchAimComponent::BeginPlay()
{
    Super::BeginPlay();

    const ARollingBallGameCharacter* RollingBall = Cast<ARollingBallGameCharacter>(GetOwner());
    Camera = RollingBall->GetCamera();
    CameraBoom = RollingBall->GetCameraBoom();
}

void ULaunchAimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

