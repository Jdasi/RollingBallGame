// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TimeDilationSubsystem.generated.h"

UCLASS()
class ROLLINGBALLGAME_API UTimeDilationSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    /** Request slo-mo or overwrite a request. 0 = slow, 1 = normal, 2 = fast. */
    void RequestDilation(const FName& Requester, const float Amount);
    void ClearRequest(const FName& Requester);
    void ClearAllRequests();

private:
    TMap<FName, float> Requests;
    float CurrentDilation = 0;

    virtual void OnWorldEndPlay(UWorld& InWorld) override;
    void RecalculateCurrentDilation();

};
