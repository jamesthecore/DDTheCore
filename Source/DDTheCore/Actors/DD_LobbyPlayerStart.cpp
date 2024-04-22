#include "Actors/DD_LobbyPlayerStart.h"

EDD_PlayerType ADD_LobbyPlayerStart::GetPlayerType() const
{
	return PlayerType;
}

bool ADD_LobbyPlayerStart::IsUsed() const
{
	return bIsUsed;
}

bool ADD_LobbyPlayerStart::IsPlayerOfType(const EDD_PlayerType Type) const
{
	return PlayerType == Type;
}
