#pragma once

#include "CoreMinimal.h"
#include "FindSessionsCallbackProxy.h"
#include "OnlineSessionDelegates.h"
#include "OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Actors/PlayerStates/DD_PlayerStateBase.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DD_OnlineGameInstanceSubsystem.generated.h"

USTRUCT()
struct FDD_SessionInformation
{
	GENERATED_BODY()

	bool bIsLanMatch = false;
	int32 MaxNumConnections = 0;
};

USTRUCT(BlueprintType)
struct FDD_SharedPlayerInformation
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		EDD_PlayerType PlayerType = EDD_PlayerType::None;
};

enum class EDD_PlayerType : uint8;
struct FBlueprintSessionResult;
class FOnlineSessionSearch;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreateSession, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSessionFound, int32, SessionResultIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJoinSession, FBlueprintSessionResult, SessionResult);

UCLASS()
class UDD_OnlineGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FCreateSession CreateSessionDelegate;
	UPROPERTY(BlueprintAssignable)
		FSessionFound SessionFoundDelegate;

	UFUNCTION(BlueprintCallable)
		void JoinOrCreateSession(EDD_PlayerType PlayerType, bool bIsLan, int32 MaxNumPlayersConnections);
	
	UFUNCTION(BlueprintCallable)
		void CreateSession(int32 MaxConnections, bool bIsLan);
	UFUNCTION(BlueprintCallable)
		void FindSession(int32 MaxResults, bool bIsLan);
	UFUNCTION(BlueprintCallable)
		void JoinSession(int32 SessionIndex);
	UFUNCTION(BlueprintCallable)
		FBlueprintSessionResult GetSessionResult(int32 SessionIndex) const;

	UDD_OnlineGameInstanceSubsystem();

	const FDD_SharedPlayerInformation& GetSharedPlayerInformation() const;
	void SetSharedPlayerInformation(const FDD_SharedPlayerInformation& NewSharedPlayerInformation);

private:
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	FDelegateHandle FindSessionCompleteDelegateHandle;
	FDelegateHandle JoinSessionCompleteDelegateHandle;

	TSharedPtr<FOnlineSessionSearch> Settings;
	FDD_SessionInformation SessionInformation;
	FDD_SharedPlayerInformation SharedPlayerInformation;

	void OnSessionCreated(FName SessionName, bool bSuccess);
	void OnSessionFound(bool bSuccess);
	void OnSessionJoined(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};