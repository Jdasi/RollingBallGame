// Fill out your copyright notice in the Description page of Project Settings.

#include "RollingBallGamePlayerState.h"
#include "RollingBallGameCharacter.h"
#include "RollingBallGamePlayerController.h"
#include "ActorComponents/BallJumpComponent.h"

void ARollingBallGamePlayerState::SetCheckpoint(AActor* InCheckpoint)
{
    Checkpoint = InCheckpoint;
}

void ARollingBallGamePlayerState::BeginPlay()
{
    Super::BeginPlay();
    AdjustMaxJumpCharges(MaxJumpCharges);
}

bool ARollingBallGamePlayerState::AdjustMaxJumpCharges(int Amount)
{
    const int PrevMaxJumpCharges = MaxJumpCharges;
    MaxJumpCharges = FMath::Clamp(MaxJumpCharges + Amount, 0, MAX_JUMP_CHARGES);

    if (MaxJumpCharges == PrevMaxJumpCharges)
    {
        return false;
    }

    const int Diff = MaxJumpCharges - PrevMaxJumpCharges;
    const ARollingBallGamePlayerController* PlayerController = Cast<ARollingBallGamePlayerController>(GetPlayerController());

    if (const ARollingBallGameCharacter* RollingBall = Cast<ARollingBallGameCharacter>(PlayerController->GetPawn()))
    {
        RollingBall->JumpComponent->AdjustJumpCharges(Diff, EJumpChargeAdjustReasons::MaxChargesChanged);

        if (Diff > 0)
        {
            RollingBall->JumpComponent->ClearJumpCooldown();
        }
    }

    return true;
}
