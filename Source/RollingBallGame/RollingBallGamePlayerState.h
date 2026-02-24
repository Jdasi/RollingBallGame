// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RollingBallGamePlayerState.generated.h"

UCLASS(Abstract)
class ROLLINGBALLGAME_API ARollingBallGamePlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    FORCEINLINE AActor* GetCheckpoint() const { return Checkpoint; }
    void SetCheckpoint(AActor* InCheckpoint);

private:
    UPROPERTY()
    AActor* Checkpoint = nullptr;
};
