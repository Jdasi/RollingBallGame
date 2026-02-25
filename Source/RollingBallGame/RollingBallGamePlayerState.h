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

    UFUNCTION(BlueprintCallable, Category = "State|RollingBall")
    bool AdjustMaxJumpCharges(int Amount);

    UFUNCTION(BlueprintCallable, Category = "State|RollingBall")
    void MoveCheckpoint(const USceneComponent* InComponent);

    UPROPERTY(EditAnywhere, Category = "Settings")
    TSubclassOf<AActor> MoveableSpawnClass = nullptr;

    FORCEINLINE AActor* GetCheckpoint() const { return Checkpoint; }
    FORCEINLINE int GetMaxJumpCharges() const { return MaxJumpCharges; }

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    AActor* MoveableSpawn = nullptr;

    UPROPERTY()
    AActor* Checkpoint = nullptr;

    UPROPERTY(VisibleAnywhere, Category="Settings", Meta=(Tooltip="Range 0-3", ClampMin="0", ClampMax="3"))
    int MaxJumpCharges = 0;
};
