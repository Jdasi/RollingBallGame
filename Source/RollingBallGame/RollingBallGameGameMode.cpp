// Copyright Epic Games, Inc. All Rights Reserved.

#include "RollingBallGameGameMode.h"
#include "RollingBallGamePlayerController.h"
#include "RollingBallGamePlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void ARollingBallGameGameMode::RespawnPlayer()
{
    if (!PlayerController || !PlayerState)
    {
        return;
    }

    if (APawn* Pawn = PlayerController->GetPawn())
    {
        GetWorld()->DestroyActor(Pawn);
    }

    AActor* Checkpoint = PlayerState->GetCheckpoint();
    RestartPlayerAtPlayerStart(PlayerController, Checkpoint);
}

void ARollingBallGameGameMode::BeginPlay()
{
    Super::BeginPlay();

    PlayerController = Cast<ARollingBallGamePlayerController>(GetWorld()->GetFirstPlayerController());
    PlayerState = PlayerController->GetPlayerState<ARollingBallGamePlayerState>();

    if (AActor* PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()))
    {
        PlayerState->SetCheckpoint(PlayerStart);
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Failed to find player start");
    }
}
