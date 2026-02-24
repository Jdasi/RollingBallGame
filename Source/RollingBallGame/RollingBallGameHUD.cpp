// Fill out your copyright notice in the Description page of Project Settings.

#include "RollingBallGameHUD.h"
#include "RollingBallGameCharacter.h"
#include "ActorComponents/BallJumpComponent.h"
#include "ActorComponents/LaunchAbilityComponent.h"
#include "UI/GameViewWidget.h"
#include "UI/JumpChargeGroupWidget.h"
#include "UI/LaunchReticleWidget.h"

void ARollingBallGameHUD::OnPossessRollingBall(ARollingBallGameCharacter* InRollingBall)
{
    if (!GameViewWidget)
    {
        GameViewWidget = CreateWidget<UGameViewWidget>(GetWorld(), GameViewClass);
        GameViewWidget->AddToViewport();
    }

    RollingBall = InRollingBall;
    RollingBall->JumpComponent->JumpChargesChanged.AddDynamic(this, &ARollingBallGameHUD::OnRollingBallJumpChargesChanged);
    RollingBall->LaunchAbilityComponent->AimStateChanged.AddDynamic(this, &ARollingBallGameHUD::OnRollingBallAimStateChanged);

    OnRollingBallJumpChargesChanged(0, RollingBall->JumpComponent->GetJumpCharges(), EJumpChargeAdjustReasons::OnPossess);
    OnRollingBallAimStateChanged(false);
}

void ARollingBallGameHUD::OnUnPossessRollingBall()
{
    if (RollingBall)
    {
        RollingBall->JumpComponent->JumpChargesChanged.RemoveDynamic(this, &ARollingBallGameHUD::OnRollingBallJumpChargesChanged);
        RollingBall->LaunchAbilityComponent->AimStateChanged.RemoveDynamic(this, &ARollingBallGameHUD::OnRollingBallAimStateChanged);
        RollingBall = nullptr;
        GameViewWidget->JumpChargeGroup->RefreshJumpCharges(0);
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
