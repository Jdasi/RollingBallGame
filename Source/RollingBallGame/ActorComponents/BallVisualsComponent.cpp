// Fill out your copyright notice in the Description page of Project Settings.

#include "BallVisualsComponent.h"
#include "LaunchAbilityComponent.h"
#include "RollingBallGameCharacter.h"

void UBallVisualsComponent::BeginPlay()
{
    Super::BeginPlay();

    const ARollingBallGameCharacter* RollingBall = Cast<ARollingBallGameCharacter>(GetOwner());
    UStaticMeshComponent* Mesh = RollingBall->GetMesh();

    DynamicMaterialInstance = Mesh->CreateDynamicMaterialInstance(0);
    Mesh->SetMaterial(0, DynamicMaterialInstance);
    DynamicMaterialInstance->GetVectorParameterValue(FName("Emissive Color"), OriginalColor);

    RollingBall->LaunchAbilityComponent->LaunchAvailabilityChanged.AddDynamic(this, &UBallVisualsComponent::OnBallLaunchAvailabilityChanged);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UBallVisualsComponent::OnBallLaunchAvailabilityChanged(bool Available)
{
    DynamicMaterialInstance->SetVectorParameterValue(FName("Emissive Color"), Available ? LaunchAvailableColor : OriginalColor);
}
