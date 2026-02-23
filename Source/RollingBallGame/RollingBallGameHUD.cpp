// Fill out your copyright notice in the Description page of Project Settings.

#include "RollingBallGameHUD.h"
#include "RollingBallGameCharacter.h"
#include "ActorComponents/BallJumpComponent.h"
#include "ActorComponents/LaunchAbilityComponent.h"
#include "UI/GameViewWidget.h"
#include "UI/JumpChargeGroupWidget.h"
#include "UI/LaunchReticleWidget.h"

void ARollingBallGameHUD::BeginPlay()
{
    Super::BeginPlay();

    GameViewWidget = CreateWidget<UGameViewWidget>(GetWorld(), GameViewClass);
    GameViewWidget->AddToViewport();

    if (const APlayerController* RollingBallController = GetOwningPlayerController())
    {
        RollingBall = Cast<ARollingBallGameCharacter>(RollingBallController->GetPawn());
        RollingBall->JumpComponent->JumpChargesChanged.AddDynamic(this, &ARollingBallGameHUD::OnRollingBallJumpChargesChanged);
        RollingBall->LaunchAbilityComponent->AimStateChanged.AddDynamic(this, &ARollingBallGameHUD::OnRollingBallAimStateChanged);

        OnRollingBallJumpChargesChanged(0, RollingBall->JumpComponent->GetJumpCharges(), EJumpChargeAdjustReasons::BeginPlay);
        OnRollingBallAimStateChanged(false);
    }
}

void ARollingBallGameHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    if (RollingBall)
    {
        RollingBall->JumpComponent->JumpChargesChanged.RemoveDynamic(this, &ARollingBallGameHUD::OnRollingBallJumpChargesChanged);
        RollingBall->LaunchAbilityComponent->AimStateChanged.RemoveDynamic(this, &ARollingBallGameHUD::OnRollingBallAimStateChanged);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ARollingBallGameHUD::OnRollingBallJumpChargesChanged(const int OldValue, const int NewValue, EJumpChargeAdjustReasons Reason)
{
    GameViewWidget->JumpChargeGroup->RefreshJumpCharges(NewValue);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ARollingBallGameHUD::OnRollingBallAimStateChanged(bool Started)
{
    GameViewWidget->LaunchReticle->SetActive(Started);
}
