// Fill out your copyright notice in the Description page of Project Settings.

#include "TimeDilationSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UTimeDilationSubsystem::RequestDilation(const FName& Requester, const float Amount)
{
    if (Amount == 1.0f)
    {
        ClearRequest(Requester);
    }
    else
    {
        auto& Val = Requests.FindOrAdd(Requester);
        Val = Amount;

        RecalculateCurrentDilation();
    }
}

void UTimeDilationSubsystem::ClearRequest(const FName& Requester)
{
    Requests.Remove(Requester);
    RecalculateCurrentDilation();
}

void UTimeDilationSubsystem::ClearAllRequests()
{
    Requests.Empty();
    RecalculateCurrentDilation();
}

void UTimeDilationSubsystem::OnWorldEndPlay(UWorld& InWorld)
{
    Super::OnWorldEndPlay(InWorld);
    ClearAllRequests();
}

void UTimeDilationSubsystem::RecalculateCurrentDilation()
{
    if (Requests.IsEmpty())
    {
        CurrentDilation = 1.0f;
    }
    else
    {
        CurrentDilation = 0.0f;

        for (const auto& Request : Requests)
        {
            CurrentDilation = FMath::Max(CurrentDilation, Request.Value);
        }
    }

    // TODO - lerp to new value?
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), CurrentDilation);
}
