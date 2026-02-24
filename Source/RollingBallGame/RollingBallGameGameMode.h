// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RollingBallGameGameMode.generated.h"

class ARollingBallGamePlayerState;
class ARollingBallGamePlayerController;

UCLASS(abstract)
class ARollingBallGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="GameMode")
	void RespawnPlayer();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ARollingBallGamePlayerController* PlayerController;

	UPROPERTY()
	ARollingBallGamePlayerState* PlayerState;
};



