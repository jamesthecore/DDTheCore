#pragma once

#include "CoreMinimal.h"
#include "DDTheCore.h"
#include "GameFramework/PlayerState.h"
#include "DD_PlayerStateBase.generated.h"

enum class EDD_PlayerType : uint8;

UCLASS()
class ADD_PlayerStateBase : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
		EDD_PlayerType GetPlayerType() const;
	
	void AddScore(int32 ScoreAmount);
	void SetPlayerType(const EDD_PlayerType Type);

	int32 GetIndividualScore() const;

protected:
	UPROPERTY(Replicated)
		EDD_PlayerType PlayerType = EDD_PlayerType::Assassin;
	UPROPERTY(Replicated)
		int32 IndividualScore = 0;
};