// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "SkiCameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class MADMOUNTAIN_API USkiCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
public:
	USkiCameraComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void InitializeComponent() override;

	void UpdateTargetFocalPoint();
	void UpdateTargetCameraLocation();
	void LerpCameraToTargets();
protected:
	UPROPERTY()
	class ASkiCharacterBase* OwningCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "CameraMovement")
	class UCurveFloat* SpeedToBoomLength;
	UPROPERTY(EditDefaultsOnly, Category = "CameraMovement")
	class UCurveFloat* SpeedToFocalTargetBoomLength;
	UPROPERTY(EditDefaultsOnly, Category = "CameraMovement")
	class UCurveFloat* SpeedToCameraCatchupSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "CameraMovement")
	FVector CameraTargetLocationOffset;

private:
	UPROPERTY(Transient)
	FVector CameraTargetLocation;
	UPROPERTY(Transient)
	FVector TargetFocalPoint;
	UPROPERTY(Transient)
	float CharacterSpeed;
};
