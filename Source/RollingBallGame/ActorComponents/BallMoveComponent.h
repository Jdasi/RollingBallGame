// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BallMoveComponent.generated.h"

class UCameraComponent;
class USphereComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROLLINGBALLGAME_API UBallMoveComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category="Settings")
    float TorqueForce = 500000.0f;

    UPROPERTY(EditAnywhere, Category="Settings")
    float MaxAngularVelocity = 30.0f;

    UBallMoveComponent();

    void Move(FVector2d Move) const;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    USphereComponent* Sphere = nullptr;

    UPROPERTY()
    UCameraComponent* Camera = nullptr;
};
