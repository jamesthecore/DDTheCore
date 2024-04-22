#pragma once

#include "CoreMinimal.h"
#include "DD_GameModeBase.h"
#include "DD_GameModeLobby.generated.h"

enum class EDD_PlayerType : uint8;

UCLASS()
class ADD_GameModeLobby : public ADD_GameModeBase
{
	GENERATED_BODY()

protected:
	virtual const AActor* GetPlayerStart(EDD_PlayerType PlayerType) const override;
};