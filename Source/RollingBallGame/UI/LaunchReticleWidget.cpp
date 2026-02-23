// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/LaunchReticleWidget.h"
#include "Components/Image.h"

void ULaunchReticleWidget::SetActive(bool Active) const
{
    ReticleDotImage->SetOpacity(Active ? 1.0f : 0.5f);
    ReticleArcImage0->SetOpacity(Active ? 1.0f : 0.0f);
    ReticleArcImage1->SetOpacity(Active ? 1.0f : 0.0f);
    ReticleArcImage2->SetOpacity(Active ? 1.0f : 0.0f);
    ReticleArcImage3->SetOpacity(Active ? 1.0f : 0.0f);
}
