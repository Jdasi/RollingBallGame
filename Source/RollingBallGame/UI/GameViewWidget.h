// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomWidget.h"
#include "GameViewWidget.generated.h"

class UJumpChargeGroupWidget;

UCLASS(Abstract)
class ROLLINGBALLGAME_API UGameViewWidget : public UCustomWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category="Constituent Controls", meta=(BindWidget))
    UJumpChargeGroupWidget* JumpChargeGroup = nullptr;

    // TODO - crosshair

};
