// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LaunchAimComponent.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROLLINGBALLGAME_API ULaunchAimComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category="Settings")
    float TimeDilation = 0.4f;

    UPROPERTY(EditAnywhere, Category="Settings")
    float ExitAimDelay = 0.5f;

    UPROPERTY(EditAnywhere, Category="Lerp Targets")
    FVector OffsetLerpTarget = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, Category="Lerp Targets")
    float FovLerpTarget = 50.0f;

    UPROPERTY(EditAnywhere, Category="Lerp Targets")
    float CameraLagLerpTarget = 1.0f;

    UPROPERTY(EditAnywhere, Category="Lerp Speeds")
    float OffsetLerpSpeed = 10.0f;

    UPROPERTY(EditAnywhere, Category="Lerp Speeds")
    float FovLerpSpeed = 10.0f;

    UPROPERTY(EditAnywhere, Category="Lerp Speeds")
    float CameraLagLerpSpeed = 10.0f;

    UPROPERTY(EditAnywhere, Category="Lerp Speeds")
    float CameraRotationLagLerpSpeed = 10.0f;

    ULaunchAimComponent();

    void StartAim();
    void EndAim();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    UCameraComponent* Camera = nullptr;

    UPROPERTY()
    USpringArmComponent* CameraBoom = nullptr;

    void SetTimeDilationActive(bool Active) const;

    FVector InitialOffset = FVector::ZeroVector;
    float InitialFov = 0.0f;
    float InitialCameraLag = 0.0f;
    float InitialCameraRotationLag = 0.0f;
    float ExitTimer = 0.0f;
    bool AimActive = false;
    bool AimRequested = false;
};
