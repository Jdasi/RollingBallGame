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
    const int MAX_JUMP_CHARGES = 3;

    FORCEINLINE AActor* GetCheckpoint() const { return Checkpoint; }
    void SetCheckpoint(AActor* InCheckpoint);

    FORCEINLINE int GetMaxJumpCharges() const { return MaxJumpCharges; }
    UFUNCTION(BlueprintCallable, Category = "State|RollingBall")
    bool AdjustMaxJumpCharges(int Amount);

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    AActor* Checkpoint = nullptr;

    UPROPERTY(VisibleAnywhere, Category="Settings", Meta=(Tooltip="Range 0-3", ClampMin="0", ClampMax="3"))
    int MaxJumpCharges = 0;
};
