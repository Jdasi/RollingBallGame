// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BallJumpComponent.h"
#include "Components/ActorComponent.h"
#include "BallAudioComponent.generated.h"

class ULaunchAbilityComponent;
class UBallJumpComponent;
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

    UPROPERTY(EditAnywhere, Category="Sound Cues")
    USoundCue* JumpSound = nullptr;

    UPROPERTY(EditAnywhere, Category="Sound Cues")
    USoundCue* JumpRechargeSound = nullptr;

    UPROPERTY(EditAnywhere, Category="Impact Settings")
    float MinImpactThreshold = 300.0f;

    UPROPERTY(EditAnywhere, Category="Impact Settings")
    float MaxImpactThreshold = 15000.0f;

    UPROPERTY(EditAnywhere, Category="Impact Settings")
    float MaxImpactVolume = 0.75f;

    UBallAudioComponent();

protected:
    UFUNCTION()
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    USphereComponent* Sphere;

    UPROPERTY()
    UBallMoveComponent* MoveComponent;

    UPROPERTY()
    UBallJumpComponent* JumpComponent;

    UPROPERTY()
    ULaunchAbilityComponent* LaunchAbilityComponent;

    UPROPERTY()
    UAudioComponent* RollingLoopAudio;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION()
    void JumpChargesChanged(int32 OldValue, int32 NewValue, EJumpChargeAdjustReasons Reason);

    void UpdateRollingAudio() const;
};
