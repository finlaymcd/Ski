// Fill out your copyright notice in the Description page of Project Settings.


#include "SkiCharacterInputComponent.h"
#include "SkiCharacterBase.h"

USkiCharacterInputComponent::USkiCharacterInputComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;
}

void USkiCharacterInputComponent::InitializeComponent()
{
	Super::InitializeComponent();
	OwningCharacter = Cast<ASkiCharacterBase>(GetOwner());
}

void USkiCharacterInputComponent::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("Push"), IE_Pressed, this, &USkiCharacterInputComponent::InputPushPressed);
	PlayerInputComponent->BindAction(TEXT("Push"), IE_Released, this, &USkiCharacterInputComponent::InputPushReleased);
	PlayerInputComponent->BindAction(TEXT("Tuck"), IE_Pressed, this, &USkiCharacterInputComponent::InputTuckPressed);
	PlayerInputComponent->BindAction(TEXT("Tuck"), IE_Released, this, &USkiCharacterInputComponent::InputTuckReleased);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &USkiCharacterInputComponent::TurnInput);
}

void USkiCharacterInputComponent::InputPushPressed()
{
 	check(OwningCharacter);
	OwningCharacter->StartPush();
}

void USkiCharacterInputComponent::InputPushReleased()
{
	check(OwningCharacter);
	OwningCharacter->EndPush();
}

void USkiCharacterInputComponent::InputTuckPressed()
{
	check(OwningCharacter);
	OwningCharacter->StartTuck();
}

void USkiCharacterInputComponent::InputTuckReleased()
{
	check(OwningCharacter);
	OwningCharacter->EndTuck();
}

void USkiCharacterInputComponent::TurnInput(float Value)
{
	check(OwningCharacter);
	if (OwningCharacter->IsLocallyControlled())
	{
		OwningCharacter->SetTurnInputValue(Value);
	}

}
