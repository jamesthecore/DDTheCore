#include "Actors/GameModes/DD_GameModeMainGame.h"

uint8 ADD_GameModeMainGame::GetMaxNumGenerators() const
{
	return MaxNumGenerators;
}

const AActor* ADD_GameModeMainGame::GetPlayerStart(EDD_PlayerType PlayerType) const
{
	return nullptr;
}

