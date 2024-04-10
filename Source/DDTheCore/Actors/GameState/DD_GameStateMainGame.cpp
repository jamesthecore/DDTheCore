#include "Actors/GameState/DD_GameStateMainGame.h"
#include "DDTheCore.h"
#include "UnrealNetwork.h"
#include "Libraries/DD_NetLibrary.h"

void ADD_GameStateMainGame::AddScore(EDD_PlayerType PlayerType)
{
	switch (PlayerType)
	{
	case EDD_PlayerType::Assassin:
		{
			AssassinScore++;
			if (UDD_NetLibrary::IsClient(this))
			{
				OnRep_AssassinScore();
			}
		}
		break;
	case EDD_PlayerType::Civilian:
		{
			CivilianScore++;
			if (UDD_NetLibrary::IsClient(this))
			{
				OnRep_CivilianScore();
			}
		}
		break;
	}
}

int32 ADD_GameStateMainGame::GetScore(EDD_PlayerType PlayerType) const
{
	switch (PlayerType)
	{
	case EDD_PlayerType::Assassin:
			return AssassinScore;
	case EDD_PlayerType::Civilian:
			return CivilianScore;
	}

	return -1;
}

void ADD_GameStateMainGame::OnRep_AssassinScore()
{
	AssassinScoreUpdated.Broadcast(AssassinScore);
}

void ADD_GameStateMainGame::OnRep_CivilianScore()
{
	CivilianScoreUpdated.Broadcast(CivilianScore);
}

void ADD_GameStateMainGame::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADD_GameStateMainGame, AssassinScore);
	DOREPLIFETIME(ADD_GameStateMainGame, CivilianScore);
}
