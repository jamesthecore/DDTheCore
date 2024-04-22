#include "GameInstances/DD_OnlineGameInstanceSubsystem.h"
#include "OnlineSessionInterface.h"
#include "OnlineSessionNames.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"
#include "Actors/PlayerStates/DD_PlayerStateBase.h"
#include "Kismet/GameplayStatics.h"

void UDD_OnlineGameInstanceSubsystem::JoinOrCreateSession(EDD_PlayerType PlayerType, bool bIsLan, int32 MaxNumPlayersConnections)
{
	SessionInformation.bIsLanMatch = bIsLan;
	SessionInformation.MaxNumConnections = MaxNumPlayersConnections;

	SharedPlayerInformation.PlayerType = PlayerType;
	
	FindSession(MaxNumPlayersConnections, bIsLan);
}

void UDD_OnlineGameInstanceSubsystem::CreateSession(int32 MaxConnections, bool bIsLan)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogOnline, Error, TEXT("UDD_OnlineGameInstanceSubsystem::CreateSession -> Session Interface is not valid."));
		return;
	}

	TSharedPtr<FOnlineSessionSettings> CreateSessionSettings = MakeShareable(new FOnlineSessionSettings());

	CreateSessionSettings->NumPrivateConnections = 0;
	CreateSessionSettings->NumPublicConnections = MaxConnections;
	CreateSessionSettings->bAllowInvites = true;
	CreateSessionSettings->bAllowJoinInProgress = true;
	CreateSessionSettings->bAllowJoinViaPresence = true;
	CreateSessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
	CreateSessionSettings->bIsDedicated = false;
	CreateSessionSettings->bIsLANMatch = bIsLan;
	CreateSessionSettings->bShouldAdvertise = true;

	CreateSessionSettings->Set(SETTING_MAPNAME, FString("Lobby"), EOnlineDataAdvertisementType::ViaOnlineService);
	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!LocalPlayer)
	{
		UE_LOG(LogOnline, Error, TEXT("UDD_OnlineGameInstanceSubsystem::CreateSession -> Local player is not valid"));
		return;
	}

	const bool bSessionCreated = SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *CreateSessionSettings);
	if (!bSessionCreated)
	{
		UE_LOG(LogOnline, Error, TEXT("UDD_OnlineGameInstanceSubsystem::CreateSession -> Session hasn't been created correctly"));
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		CreateSessionDelegate.Broadcast(false);
		return;
	}
}

void UDD_OnlineGameInstanceSubsystem::FindSession(int32 MaxResults, bool bIsLan)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface)
	{
		UE_LOG(LogOnline, Error, TEXT("UDD_OnlineGameInstanceSubsystem::FindSession -> Interface is not valid"));
		return;
	}

	FindSessionCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);
	Settings = MakeShareable(new FOnlineSessionSearch());

	Settings->MaxSearchResults = MaxResults;
	Settings->bIsLanQuery = bIsLan;

	Settings->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!LocalPlayer)
	{
		UE_LOG(LogOnline, Error, TEXT("UDD_OnlineGameInstanceSubsystem::FindSession -> Local player is not valid"));
		return;
	}

	const bool bSessionsFound = SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), Settings.ToSharedRef());
	if (!bSessionsFound)
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegateHandle);
	}
}

void UDD_OnlineGameInstanceSubsystem::JoinSession(int32 SessionIndex)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface)
	{
		UE_LOG(LogOnline, Error, TEXT("UDD_OnlineGameInstanceSubsystem::JoinSession -> Online Interface is not valid"));
		return;
	}
	
	JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!LocalPlayer)
	{
		UE_LOG(LogOnline, Error, TEXT("UDD_OnlineGameInstanceSubsystem::JoinSession -> LocalPlayer is not valid"));
		return;
	}

	const bool bJoinedSuccessfully = SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Settings->SearchResults[SessionIndex]);
	if (!bJoinedSuccessfully)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}
}

FBlueprintSessionResult UDD_OnlineGameInstanceSubsystem::GetSessionResult(int32 SessionIndex) const
{
	FBlueprintSessionResult Result;
	if (SessionIndex >= Settings->SearchResults.Num())
		return Result;

	Result.OnlineResult = Settings->SearchResults[SessionIndex];
	return Result;
}

UDD_OnlineGameInstanceSubsystem::UDD_OnlineGameInstanceSubsystem()
	: CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnSessionCreated)),
	FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnSessionFound)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnSessionJoined))
{
}

const FDD_SharedPlayerInformation& UDD_OnlineGameInstanceSubsystem::GetSharedPlayerInformation() const
{
	return SharedPlayerInformation;
}

void UDD_OnlineGameInstanceSubsystem::SetSharedPlayerInformation(const FDD_SharedPlayerInformation& NewSharedPlayerInformation)
{
	SharedPlayerInformation = NewSharedPlayerInformation;
}

void UDD_OnlineGameInstanceSubsystem::OnSessionCreated(FName Name, bool bArg)
{
	if (bArg)
	{
		UE_LOG(LogOnline, Warning, TEXT("Created session with name %s"), *(Name.ToString()));
	}
	else
	{
		UE_LOG(LogOnline, Error, TEXT("Failed Creating session with name %s"), *(Name.ToString()));
	}

	CreateSessionDelegate.Broadcast(bArg);
}

void UDD_OnlineGameInstanceSubsystem::OnSessionFound(bool bArg)
{
	if (bArg)
	{
		if (Settings->SearchResults.Num() > 0)
		{
			for (int32 Index = 0; Index < Settings->SearchResults.Num(); ++Index)
			{
				SessionFoundDelegate.Broadcast(Index);
			}
		}
		else
		{
			CreateSession(SessionInformation.MaxNumConnections, SessionInformation.bIsLanMatch);
		}
	}
}

void UDD_OnlineGameInstanceSubsystem::OnSessionJoined(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface)
	{
		UE_LOG(LogOnline, Error, TEXT("UDD_OnlineGameInstanceSubsystem::OnSessionJoined -> Online Interface is not valid"));
		return;
	}
	
	FString ConnectString;
	const bool bConnectionSuccess = SessionInterface->GetResolvedConnectString(SessionName, ConnectString);
	if (!bConnectionSuccess)
	{
		UE_LOG(LogOnline, Error, TEXT("UDD_OnlineGameInstanceSubsystem::OnSessionJoined -> Couldn't resolve connection"));
		return;
	}

	if (ADD_PlayerStateBase* PlayerState = Cast<ADD_PlayerStateBase>(UGameplayStatics::GetPlayerState(this, 0)))
	{
		PlayerState->SetPlayerType(EDD_PlayerType::Civilian);
	}
	
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->ClientTravel(ConnectString, TRAVEL_Absolute);
	}
}