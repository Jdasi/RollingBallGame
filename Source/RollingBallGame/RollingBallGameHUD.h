// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RollingBallGameHUD.generated.h"

enum EJumpChargeAdjustReasons : uint8;
class ARollingBallGameCharacter;
class UGameViewWidget;

UCLASS()
class ROLLINGBALLGAME_API ARollingBallGameHUD : public AHUD
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    TSubclassOf<UGameViewWidget> GameViewClass = nullptr;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    UPROPERTY()
    UGameViewWidget* GameViewWidget = nullptr;

    UPROPERTY()
    ARollingBallGameCharacter* RollingBall = nullptr;

    UFUNCTION()
    void OnRollingBallJumpChargesChanged(int OldValue, int NewValue, EJumpChargeAdjustReasons Reason);
};
