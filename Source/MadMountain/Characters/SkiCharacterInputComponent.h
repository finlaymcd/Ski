// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkiCharacterInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class MADMOUNTAIN_API USkiCharacterInputComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class ASkiCharacterBase;
public:
	USkiCharacterInputComponent(const FObjectInitializer& ObjectInitializer);
	virtual void InitializeComponent() override;
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
protected:
	void InputPushPressed();
	void InputPushReleased();
	void InputTuckPressed();
	void InputTuckReleased();
	void TurnInput(float Value);
private:
	class ASkiCharacterBase* OwningCharacter;
};
