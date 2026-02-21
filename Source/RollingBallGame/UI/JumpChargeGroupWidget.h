// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomWidget.h"
#include "JumpChargeGroupWidget.generated.h"

class UJumpChargeWidget;

UCLASS(Abstract)
class ROLLINGBALLGAME_API UJumpChargeGroupWidget : public UCustomWidget
{
    GENERATED_BODY()

public:
    void RefreshJumpCharges(const int Amount) const;

private:
    UPROPERTY(EditAnywhere, Category="Constituent Controls", meta=(BindWidget))
    UJumpChargeWidget* Charge0;

    UPROPERTY(EditAnywhere, Category="Constituent Controls", meta=(BindWidget))
    UJumpChargeWidget* Charge1;

    UPROPERTY(EditAnywhere, Category="Constituent Controls", meta=(BindWidget))
    UJumpChargeWidget* Charge2;
};
