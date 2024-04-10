#include "Actors/GameModes/DD_GameModeMainGame.h"

#include "Actors/PlayerStates/DD_PlayerStateBase.h"
#include "GameFramework/Character.h"

uint8 ADD_GameModeMainGame::GetMaxNumGenerators() const
{
	return MaxNumGenerators;
}

void ADD_GameModeMainGame::PostLogin(APlayerController* NewPlayer)
{
	if (NewPlayer)
	{
		const EDD_PlayerType TypeToSpawn = bAssassinSet ? EDD_PlayerType::Civilian : EDD_PlayerType::Assassin;
		if (!bAssassinSet)
		{
			bAssassinSet = true;
		}

		if (ACharacter* SpawnedCharacter = GetWorld()->SpawnActor<ACharacter>(CustomSpawnData[TypeToSpawn].PawnToSpawn, NewPlayer->GetSpawnLocation(), NewPlayer->GetControlRotation()))
		{
			APawn* CurrentPawn = NewPlayer->GetPawn();
			NewPlayer->UnPossess();

			if (CurrentPawn)
			{
				CurrentPawn->Destroy();
			}

			NewPlayer->Possess(SpawnedCharacter);

			if (ADD_PlayerStateBase* PlayerStateBase = NewPlayer->GetPlayerState<ADD_PlayerStateBase>())
			{
				PlayerStateBase->SetPlayerType(TypeToSpawn);
			}
		}
	}

	Super::PostLogin(NewPlayer);
}
