// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BallAudioComponent.generated.h"

class USphereComponent;
class UBallMoveComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROLLINGBALLGAME_API UBallAudioComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category="Sound Cues")
    USoundCue* RollingSound = nullptr;

    UPROPERTY(EditAnywhere, Category="Sound Cues")
    USoundCue* ImpactSound = nullptr;

    UPROPERTY(EditAnywhere, Category="Impact Settings")
    float MinImpactThreshold = 300.0f;

    UPROPERTY(EditAnywhere, Category="Impact Settings")
    float MaxImpactThreshold = 15000.0f;

    UPROPERTY(EditAnywhere, Category="Impact Settings")
    float MaxImpactVolume = 0.75f;

    UBallAudioComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    USphereComponent* Sphere;

    UPROPERTY()
    UBallMoveComponent* MoveComponent;

    UPROPERTY()
    UAudioComponent* RollingLoopAudio;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    void UpdateRollingAudio() const;
};
