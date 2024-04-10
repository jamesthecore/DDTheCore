#include "Actors/PlayerStates/DD_PlayerStateBase.h"
#include "UnrealNetwork.h"

void ADD_PlayerStateBase::AddScore(int32 ScoreAmount)
{
	IndividualScore += ScoreAmount;
}

EDD_PlayerType ADD_PlayerStateBase::GetPlayerType() const
{
	return PlayerType;
}

int32 ADD_PlayerStateBase::GetIndividualScore() const
{
	return IndividualScore;
}

void ADD_PlayerStateBase::SetPlayerType(const EDD_PlayerType Type)
{
	PlayerType = Type;
}

void ADD_PlayerStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADD_PlayerStateBase, PlayerType);
	DOREPLIFETIME(ADD_PlayerStateBase, IndividualScore);
}
