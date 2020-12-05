// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameLiftServerSDK.h"
#include "GameFramework/GameModeBase.h"
#include "SkiGameModeBase.generated.h"

/**
 * 
 */
USTRUCT()
struct FStartGameSessionState
{
	GENERATED_BODY();

	UPROPERTY()
	bool Status;

	FStartGameSessionState()
	{
		Status = false;
	}
};

USTRUCT()
struct FUpdateGameSessionState
{
	GENERATED_BODY();

	FUpdateGameSessionState()
	{

	}
};

USTRUCT()
struct FProcessTerminateState
{
	GENERATED_BODY();
	UPROPERTY()
	bool Status;

	long TerminationTime;

	FProcessTerminateState()
	{
		Status = false;
	}
};

USTRUCT()
struct FHealthCheckState
{
	GENERATED_BODY();
	UPROPERTY()
	bool Status;

	FHealthCheckState()
	{
		Status = false;
	}
};

UCLASS()
class MADMOUNTAIN_API ASkiGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ASkiGameModeBase();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	FStartGameSessionState StartGameSessionState;

	UPROPERTY()
	FUpdateGameSessionState UpdateGameSessionState;

	UPROPERTY()
	FProcessTerminateState ProcessTerminateState;

	UPROPERTY()
	FHealthCheckState HealthCheckState;
};
