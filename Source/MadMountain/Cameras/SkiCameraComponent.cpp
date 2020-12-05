// Fill out your copyright notice in the Description page of Project Settings.


#include "SkiCameraComponent.h"

#include "Characters/SkiCharacterBase.h"

#include "Kismet/KismetMathLibrary.h"

USkiCameraComponent::USkiCameraComponent()
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void USkiCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	FDetachmentTransformRules rules = FDetachmentTransformRules::KeepWorldTransform;
	DetachFromComponent(rules);
}

void USkiCameraComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CharacterSpeed = OwningCharacter->GetCurrentSpeed();
	UpdateTargetFocalPoint();
	UpdateTargetCameraLocation();
	LerpCameraToTargets();
}

void USkiCameraComponent::InitializeComponent()
{
	Super::InitializeComponent();
	OwningCharacter = Cast<ASkiCharacterBase>(GetOwner());
}

void USkiCameraComponent::UpdateTargetFocalPoint()
{
	if (SpeedToFocalTargetBoomLength)
	{
		const float BoomLength = SpeedToFocalTargetBoomLength->GetFloatValue(CharacterSpeed);
		TargetFocalPoint = OwningCharacter->GetMesh()->GetComponentLocation() + (OwningCharacter->GetMesh()->GetRightVector() * BoomLength);
	}
}

void USkiCameraComponent::UpdateTargetCameraLocation()
{
	if (SpeedToBoomLength)
	{
		const float BoomLength = SpeedToBoomLength->GetFloatValue(CharacterSpeed);
		FVector BoomVector = OwningCharacter->GetMesh()->GetRightVector() * BoomLength * -1;
		BoomVector += CameraTargetLocationOffset;
		CameraTargetLocation = OwningCharacter->GetMesh()->GetComponentLocation() + (BoomVector);
	}
}

void USkiCameraComponent::LerpCameraToTargets()
{
	if (SpeedToCameraCatchupSpeed)
	{
		const float LerpValue = SpeedToCameraCatchupSpeed->GetFloatValue(CharacterSpeed);

		const FVector NewLocation = FMath::Lerp(GetComponentLocation(), CameraTargetLocation, LerpValue);
		SetWorldLocation(NewLocation);
		const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(NewLocation, TargetFocalPoint);
		const FRotator NewRotation = FMath::Lerp(GetComponentRotation(), TargetRotation, LerpValue);
		SetWorldRotation(NewRotation);
	}
	
}
