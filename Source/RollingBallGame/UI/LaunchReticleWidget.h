// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CustomWidget.h"
#include "LaunchReticleWidget.generated.h"

class UImage;

UCLASS(Abstract)
class ROLLINGBALLGAME_API ULaunchReticleWidget : public UCustomWidget
{
	GENERATED_BODY()

public:
	void SetActive(bool Active) const;

private:
	UPROPERTY(EditAnywhere, Category="Constituent Controls", meta=(BindWidget))
	UImage* ReticleDotImage = nullptr;

	UPROPERTY(EditAnywhere, Category="Constituent Controls", meta=(BindWidget))
	UImage* ReticleArcImage0 = nullptr;

	UPROPERTY(EditAnywhere, Category="Constituent Controls", meta=(BindWidget))
	UImage* ReticleArcImage1 = nullptr;

	UPROPERTY(EditAnywhere, Category="Constituent Controls", meta=(BindWidget))
	UImage* ReticleArcImage2 = nullptr;

	UPROPERTY(EditAnywhere, Category="Constituent Controls", meta=(BindWidget))
	UImage* ReticleArcImage3 = nullptr;
	
};
