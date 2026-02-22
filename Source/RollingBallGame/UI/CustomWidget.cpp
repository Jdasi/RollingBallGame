// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomWidget.h"

#if WITH_EDITOR
const FText UCustomWidget::GetPaletteCategory()
{
    return NSLOCTEXT("UMG", "CustomPaletteCategory", "RollingBallGame");
}
#endif
