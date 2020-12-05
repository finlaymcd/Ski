// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CompletedPush_AnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class MADMOUNTAIN_API UCompletedPush_AnimNotify : public UAnimNotify
{
	GENERATED_BODY()
		virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
