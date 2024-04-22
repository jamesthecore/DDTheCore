#include "Actors/GameModes/DD_GameModeLobby.h"

#include "Actors/DD_LobbyPlayerStart.h"
#include "Kismet/GameplayStatics.h"

const AActor* ADD_GameModeLobby::GetPlayerStart(EDD_PlayerType PlayerType) const
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, ADD_LobbyPlayerStart::StaticClass(), OutActors);

	if (OutActors.IsEmpty())
		return nullptr;

	const AActor* ResultActor = *OutActors.FindByPredicate([&PlayerType](const AActor* ActorInArray)
	{
		const ADD_LobbyPlayerStart* PlayerStart = Cast<ADD_LobbyPlayerStart>(ActorInArray);
		return PlayerStart && !PlayerStart->IsUsed() && PlayerStart->IsPlayerOfType(PlayerType);
	});

	if (!ResultActor)
	{
		UE_LOG(LogTemp, Error, TEXT("Didn't find a good start"));
	}
	return ResultActor;
}
