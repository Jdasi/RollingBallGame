// Fill out your copyright notice in the Description page of Project Settings.

#include "RollingBallGameHUD.h"
#include "RollingBallGameCharacter.h"
#include "ActorComponents/BallJumpComponent.h"
#include "UI/GameViewWidget.h"
#include "UI/JumpChargeGroupWidget.h"

void ARollingBallGameHUD::BeginPlay()
{
    Super::BeginPlay();

    GameViewWidget = CreateWidget<UGameViewWidget>(GetWorld(), GameViewClass);
    GameViewWidget->AddToViewport();

    if (const APlayerController* RollingBallController = GetOwningPlayerController())
    {
        RollingBall = Cast<ARollingBallGameCharacter>(RollingBallController->GetPawn());
        RollingBall->JumpComponent->JumpChargesChanged.AddDynamic(this, &ARollingBallGameHUD::OnRollingBallJumpChargesChanged);
    }
}

void ARollingBallGameHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    if (RollingBall)
    {
        RollingBall->JumpComponent->JumpChargesChanged.RemoveDynamic(this, &ARollingBallGameHUD::OnRollingBallJumpChargesChanged);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ARollingBallGameHUD::OnRollingBallJumpChargesChanged(const int OldValue, const int NewValue, EJumpChargeAdjustReasons Reason)
{
    GameViewWidget->JumpChargeGroup->RefreshJumpCharges(NewValue);
}
