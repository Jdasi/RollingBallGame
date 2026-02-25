// Fill out your copyright notice in the Description page of Project Settings.

#include "RollingBallGamePlayerState.h"
#include "RollingBallGameCharacter.h"
#include "RollingBallGamePlayerController.h"
#include "ActorComponents/BallJumpComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void ARollingBallGamePlayerState::BeginPlay()
{
    Super::BeginPlay();
    AdjustMaxJumpCharges(MaxJumpCharges);

    if (AActor* PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()))
    {
        Checkpoint = PlayerStart;
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Failed to find player start");
    }
}

void ARollingBallGamePlayerState::MoveCheckpoint(const USceneComponent* InComponent)
{
    if (!MoveableSpawn)
    {
        MoveableSpawn = GetWorld()->SpawnActor<AActor>(MoveableSpawnClass, InComponent->GetComponentLocation(), InComponent->GetComponentRotation());
        Checkpoint = MoveableSpawn;
    }

    Checkpoint->SetActorLocation(InComponent->GetComponentLocation());
    Checkpoint->SetActorRotation(InComponent->GetComponentRotation());
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
