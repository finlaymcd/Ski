// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SkiCharacterBase.generated.h"

UENUM(BlueprintType)
enum ECharacterState
{
	Idle     UMETA(DisplayName = "Idle"),
	Tucking      UMETA(DisplayName = "Tucking"),
	Pushing   UMETA(DisplayName = "Pushing"),
	Ragdoll		UMETA(DisplayName = "Ragdoll")
};

UCLASS()
class MADMOUNTAIN_API ASkiCharacterBase : public ACharacter
{
	GENERATED_BODY()
	friend class USkiCharacterInputComponent;
public:
	// Sets default values for this character's properties
	ASkiCharacterBase(const FObjectInitializer& ObjectInitializer);


	UFUNCTION(Server, Reliable, WithValidation)
	void StartPush();
	bool StartPush_Validate() { return true; };
	UFUNCTION(Server, Reliable, WithValidation)
	void EndPush();
	bool EndPush_Validate() { return true; };
	UFUNCTION(Server, Reliable, WithValidation)
	void StartTuck();
	bool StartTuck_Validate() { return true; };
	UFUNCTION(Server, Reliable, WithValidation)
	void EndTuck();
	bool EndTuck_Validate() { return true; };
	UFUNCTION(BlueprintPure)
	ECharacterState GetCurrentState() const { return CurrentState; }

	float GetCurrentSpeed() { return (GetActorLocation() - PreviousLocation).Size(); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetState(ECharacterState State);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void DetermineCurrentState();
	void OnCompletedPush();
	void SetTurnInputValue(float Value);
protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void AddPushForce();
	bool AddPushForce_Validate() { return true; };
	UFUNCTION(Server, Unreliable, WithValidation)
	void SetTurnInput_Server(float Value);
	bool SetTurnInput_Server_Validate(float Value) { return true; };

	void HandleTurning();
	void CalculateCurrentLeanValue(const float DeltaTime);
	UFUNCTION(BlueprintPure)
	float GetCurrentLeanValue() const {return CurrentLeanValue;}


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkiCharacterInputComponent* CharacterInputComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* PhysicsSlidingComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkiCharacterAnimComponent* CharacterAnimComponent;

	UPROPERTY(EditDefaultsOnly, Category = "SkiCharacter")
	class UPhysicalMaterial* SkiMaterial;
	UPROPERTY(EditDefaultsOnly, Category = "SkiCharacter")
	float PolePushForce = 100000.0f;
	UPROPERTY(EditDefaultsOnly, Category = "SkiCharacter")
	float LeanSpeed = 100.0f;
	UPROPERTY(EditDefaultsOnly, Category = "SkiCharacter")
	class UCurveFloat* SpeedToTurnCurve;
	UPROPERTY(EditDefaultsOnly, Category = "SkiCharacter")
	class UCurveFloat* SpeedToRotationCurve;

private:
	UPROPERTY(Transient)
	float CurrentTurnInputValue = 0.0f;
	UPROPERTY(Replicated)
	float CurrentLeanValue = 0.0f;
	UPROPERTY(Transient)
	FVector PreviousLocation = FVector::ZeroVector;
	UPROPERTY(Replicated)
	TEnumAsByte<ECharacterState> CurrentState;

	bool bWantsToPush = false;
	bool bWantsToTuck = false;
};
