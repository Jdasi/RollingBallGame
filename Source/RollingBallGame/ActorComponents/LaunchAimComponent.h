// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LaunchAimComponent.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROLLINGBALLGAME_API ULaunchAimComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    ULaunchAimComponent();

    void StartAim() const;
    void EndAim() const;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    UCameraComponent* Camera = nullptr;

    UPROPERTY()
    USpringArmComponent* CameraBoom = nullptr;
};
