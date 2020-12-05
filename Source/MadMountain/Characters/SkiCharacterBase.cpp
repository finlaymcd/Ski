// Fill out your copyright notice in the Description page of Project Settings.


#include "SkiCharacterBase.h"
#include "SkiCharacterInputComponent.h"
#include "Cameras/SkiCameraComponent.h"
#include "Animation/SkiCharacterAnimComponent.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveFloat.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values
ASkiCharacterBase::ASkiCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CharacterInputComponent = CreateOptionalDefaultSubobject<USkiCharacterInputComponent>(TEXT("CharacterInputComponent"));
	if (CharacterInputComponent)
	{
		CharacterInputComponent->SetIsReplicated(false);
	}
	CameraComponent = CreateOptionalDefaultSubobject<USkiCameraComponent>(TEXT("CameraComponent"));
	if (CameraComponent)
	{
		CameraComponent->SetIsReplicated(false);
	}
	CameraComponent->SetupAttachment(RootComponent);
	PhysicsSlidingComponent = CreateOptionalDefaultSubobject<UBoxComponent>(TEXT("PhysicsSlidingComponent"));
	if (PhysicsSlidingComponent)
	{
		PhysicsSlidingComponent->SetIsReplicated(false);
	}
	PhysicsSlidingComponent->SetupAttachment(RootComponent);
	CharacterAnimComponent = CreateOptionalDefaultSubobject<USkiCharacterAnimComponent>(TEXT("CharacterAnimComponent"));
	if (CameraComponent)
	{
		CameraComponent->SetIsReplicated(true);
	}
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASkiCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	PhysicsSlidingComponent->SetPhysMaterialOverride(SkiMaterial);
}

void ASkiCharacterBase::DetermineCurrentState()
{
	if (bWantsToTuck)
	{
		SetState(ECharacterState::Tucking);
	}
	else if (bWantsToPush)
	{
		SetState(ECharacterState::Pushing);
	}
	else
	{
		SetState(ECharacterState::Idle);
	}
}

void ASkiCharacterBase::SetState(ECharacterState State)
{
	CurrentState = State;
}

// Called every frame
void ASkiCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FMath::Abs(CurrentTurnInputValue) > 0.0f && HasAuthority())
	{
		HandleTurning();
	}
	if (HasAuthority())
	{
		CalculateCurrentLeanValue(DeltaTime);
	}
}

void ASkiCharacterBase::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	CharacterInputComponent->SetupPlayerInputComponent(PlayerInputComponent);
}

void ASkiCharacterBase::StartPush_Implementation()
{
	check(CharacterAnimComponent);
	bWantsToPush = true;
	DetermineCurrentState();
}

void ASkiCharacterBase::EndPush_Implementation()
{
	check(CharacterAnimComponent);
	bWantsToPush = false;
	DetermineCurrentState();
}

void ASkiCharacterBase::OnCompletedPush()
{
	if (IsLocallyControlled())
	{
		AddPushForce();
	}
}

void ASkiCharacterBase::SetTurnInputValue(float Value)
{
	if (IsLocallyControlled())
	{
		SetTurnInput_Server(Value);
		CurrentTurnInputValue = Value;
	}
}

void ASkiCharacterBase::StartTuck_Implementation()
{
	check(CharacterAnimComponent);
	bWantsToTuck = true;
	DetermineCurrentState();
}

void ASkiCharacterBase::EndTuck_Implementation()
{
	check(CharacterAnimComponent);
	bWantsToTuck = false;
	DetermineCurrentState();
}

void ASkiCharacterBase::SetTurnInput_Server_Implementation(float Value)
{
	CurrentTurnInputValue = Value;
}

void ASkiCharacterBase::HandleTurning()
{
	const float ForwardSpeed = UKismetMathLibrary::Quat_UnrotateVector(GetActorRotation().Quaternion(), GetVelocity()).X;
	if (SpeedToTurnCurve)
	{
		float MaxTurnValue = SpeedToTurnCurve->GetFloatValue(ForwardSpeed);
		const float TurnValue = (CurrentLeanValue - 50) * MaxTurnValue;
		GetCapsuleComponent()->AddForce(GetActorRightVector() * TurnValue);
	}

	const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(PreviousLocation, GetActorLocation());
	if (SpeedToRotationCurve)
	{
		const FRotator NewRotation = FMath::Lerp(GetActorRotation(), LookAtRot, SpeedToRotationCurve->GetFloatValue(GetVelocity().Size()));

		SetActorRotation(NewRotation, ETeleportType::TeleportPhysics);
	}

	PreviousLocation = GetActorLocation();
}

void ASkiCharacterBase::CalculateCurrentLeanValue(const float DeltaTime)
{
	if (CurrentTurnInputValue == 0.0f)
	{
		const bool LeaningLeft = CurrentLeanValue < 50.0f;
		if (CurrentLeanValue == 50.0f)
		{
			return;
		}
		if (LeaningLeft)
		{
			CurrentLeanValue += (DeltaTime * LeanSpeed);
		}
		else
		{
			CurrentLeanValue -= (DeltaTime * LeanSpeed);
		}
		if (LeaningLeft != (CurrentLeanValue < 50.0f))
		{
			CurrentLeanValue = 50.0f;
		}
	}
	else
	{
		CurrentLeanValue += (DeltaTime * LeanSpeed * CurrentTurnInputValue);
	}
	CurrentLeanValue = FMath::Clamp(CurrentLeanValue, 0.0f, 100.0f);
}

void ASkiCharacterBase::AddPushForce_Implementation()
{
	const FVector PushVector = GetActorForwardVector() * PolePushForce;
	GetCapsuleComponent()->AddImpulse(PushVector);
}

void ASkiCharacterBase::GetLifetimeReplicatedProps(TArray <class FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ASkiCharacterBase, CurrentLeanValue);
	DOREPLIFETIME(ASkiCharacterBase, CurrentState);
}



