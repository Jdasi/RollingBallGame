// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomWidget.h"
#include "JumpChargeWidget.generated.h"

class UImage;

UCLASS()
class ROLLINGBALLGAME_API UJumpChargeWidget : public UCustomWidget
{
    GENERATED_BODY()

public:
    void SetVisible(bool Visible) const;

private:
    UPROPERTY(EditAnywhere, Category="Constituent Controls", meta=(BindWidget))
    UImage* IconImage = nullptr;

};
