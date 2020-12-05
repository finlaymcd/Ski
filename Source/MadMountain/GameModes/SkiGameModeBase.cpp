// Fill out your copyright notice in the Description page of Project Settings.


#include "SkiGameModeBase.h"
#include "Characters/SkiCharacterBase.h"
#include "Controllers/SkiPlayerControllerBase.h"
#include "GameModes/SkiGameStateBase.h"
#include "Controllers/SkiPlayerStateBase.h"

ASkiGameModeBase::ASkiGameModeBase()
{
	DefaultPawnClass = ASkiCharacterBase::StaticClass();
	PlayerControllerClass = ASkiPlayerControllerBase::StaticClass();
	GameStateClass = ASkiGameStateBase::StaticClass();
	PlayerStateClass = ASkiPlayerStateBase::StaticClass();

}

void ASkiGameModeBase::BeginPlay()
{
	Super::BeginPlay();

#if WITH_GAMELIFT
	auto InitSDKOutCome = Aws::GameLift::Server::InitSDK();
	if (InitSDKOutCome.IsSuccess())
	{
		auto OnStartGameSession = [](Aws::GameLift::Server::Model::GameSession GameSessionObj, void* Params)
		{
			FStartGameSessionState* State = (FStartGameSessionState*)Params;

			State->Status = Aws::GameLift::Server::ActivateGameSession().IsSuccess();
		};

		auto OnUpdateGameSession = [](Aws::GameLift::Server::Model::UpdateGameSession UpdateGameSessionObj, void* Params)
		{
			FUpdateGameSessionState* State = (FUpdateGameSessionState*)Params;
		};

		auto OnProcessTerminate = [](void* Params)
		{
			FProcessTerminateState* State = (FProcessTerminateState*)Params;
			auto GetTerminationTimeOutcome = Aws::GameLift::Server::GetTerminationTime();
			if (GetTerminationTimeOutcome.IsSuccess())
			{
				State->TerminationTime = GetTerminationTimeOutcome.GetResult();
			}
			auto ProcessEndingOutcome = Aws::GameLift::Server::ProcessEnding();
			if (ProcessEndingOutcome.IsSuccess())
			{
				State->Status = true;
				FGenericPlatformMisc::RequestExit(false);
			}
		};

		auto OnHealthCheck = [](void* Params)
		{
			FHealthCheckState* State = (FHealthCheckState*)Params;
			State->Status = true;

			return State->Status;
		};
		TArray<FString> CommandLineTokens;
		TArray<FString> CommandLineSwitches;
		int Port = FURL::UrlConfig.DefaultPort;

		FCommandLine::Parse(FCommandLine::Get(), CommandLineTokens, CommandLineSwitches);
		for (FString Str : CommandLineSwitches)
		{
			FString Key;
			FString Value;
			if (Str.Split("=", &Key, &Value))
			{
				if (Key.Equals("port"))
				{
					Port = FCString::Atoi(*Value);
				}
			}
		}

		const char* LogFile = "aLogFile.txt";
		const char** LogFiles = &LogFile;
		auto LogParams = new Aws::GameLift::Server::LogParameters(LogFiles, 1);

		auto Params = new Aws::GameLift::Server::ProcessParameters
		(
			OnStartGameSession,
			&StartGameSessionState,
			OnUpdateGameSession,
			&UpdateGameSessionState,
			OnProcessTerminate,
			&ProcessTerminateState,
			OnHealthCheck,
			&HealthCheckState,
			Port,
			*LogParams
		);

		auto ProcessReadyOutcome = Aws::GameLift::Server::ProcessReady(*Params);
	}
#endif
}
