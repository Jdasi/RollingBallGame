// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BallAudioComponent.generated.h"

class UBallMoveComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROLLINGBALLGAME_API UBallAudioComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category="Settings")
    USoundCue* RollingSound = nullptr;

    UBallAudioComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    UBallMoveComponent* MoveComponent;

    UPROPERTY()
    UAudioComponent* RollingLoopAudio;

    void UpdateRollingAudio() const;
};
