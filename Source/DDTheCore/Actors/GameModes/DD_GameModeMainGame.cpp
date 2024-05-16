#include "Actors/GameModes/DD_GameModeMainGame.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

uint8 ADD_GameModeMainGame::GetMaxNumGenerators() const
{
	return MaxNumGenerators;
}

const AActor* ADD_GameModeMainGame::GetPlayerStart(EDD_PlayerType PlayerType) const
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), OutActors);

	if (OutActors.IsEmpty())
		return nullptr;

	return OutActors[0];
}

