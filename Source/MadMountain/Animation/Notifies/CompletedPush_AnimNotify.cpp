// Fill out your copyright notice in the Description page of Project Settings.


#include "CompletedPush_AnimNotify.h"
#include "Characters/SkiCharacterBase.h"

void UCompletedPush_AnimNotify::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	ASkiCharacterBase* OwningCharacter = Cast<ASkiCharacterBase>(MeshComp->GetOwner());
	if (OwningCharacter)
	{
		OwningCharacter->OnCompletedPush();
	}
}
