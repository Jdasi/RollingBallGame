// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PowerReceiverInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UPowerReceiverInterface : public UInterface
{
	GENERATED_BODY()
};

class ROLLINGBALLGAME_API IPowerReceiverInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PowerReceiver")
	void OnPowerLost();
};
