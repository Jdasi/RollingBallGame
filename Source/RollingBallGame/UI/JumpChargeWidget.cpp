// Fill out your copyright notice in the Description page of Project Settings.

#include "JumpChargeWidget.h"
#include "Components/Image.h"

void UJumpChargeWidget::SetVisible(bool Visible) const
{
    IconImage->SetOpacity(Visible ? 1.0f : 0.0f);
}
