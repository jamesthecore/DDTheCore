#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DD_GameStateMainGame.generated.h"

enum class EDD_PlayerType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreUpdated, int32, Score);
DECLARE_MULTICAST_DELEGATE_OneParam(FGeneratorServer, AActor*);

UCLASS()
class ADD_GameStateMainGame : public AGameState
{
	GENERATED_BODY()

public:
	FGeneratorServer GeneratorServerStarted;
	FGeneratorServer GeneratorServerStop;

	UPROPERTY(BlueprintAssignable)
		FScoreUpdated AssassinScoreUpdated;
	UPROPERTY(BlueprintAssignable)
		FScoreUpdated CivilianScoreUpdated;

	void AddScore(EDD_PlayerType PlayerType);
	int32 GetScore(EDD_PlayerType PlayerType) const;

protected:
	UPROPERTY(ReplicatedUsing=OnRep_AssassinScore)
		int32 AssassinScore = 0;
	UPROPERTY(ReplicatedUsing=OnRep_CivilianScore)
		int32 CivilianScore = 0;

	UFUNCTION()
		void OnRep_AssassinScore();
	UFUNCTION()
		void OnRep_CivilianScore();
};