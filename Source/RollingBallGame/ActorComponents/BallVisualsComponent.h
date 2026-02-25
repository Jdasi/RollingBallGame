// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BallVisualsComponent.generated.h"

class USphereComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROLLINGBALLGAME_API UBallVisualsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category="Settings")
    FLinearColor LaunchAvailableColor = FLinearColor::White;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UMaterialInstanceDynamic* DynamicMaterialInstance = nullptr;

    UFUNCTION()
    void OnBallLaunchAvailabilityChanged(bool Available);

    FLinearColor OriginalColor = FLinearColor::White;
};
