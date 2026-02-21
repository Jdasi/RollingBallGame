// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomWidget.generated.h"

UCLASS()
class ROLLINGBALLGAME_API UCustomWidget : public UUserWidget
{
public:
#if WITH_EDITOR
    virtual const FText GetPaletteCategory() override;
#endif

private:
    GENERATED_BODY()
};
