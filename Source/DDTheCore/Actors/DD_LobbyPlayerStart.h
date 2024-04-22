#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "DD_LobbyPlayerStart.generated.h"

enum class EDD_PlayerType : uint8;

UCLASS()
class ADD_LobbyPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	EDD_PlayerType GetPlayerType() const;
	bool IsUsed() const;
	bool IsPlayerOfType(EDD_PlayerType Type) const;
	
protected:
	UPROPERTY(EditAnywhere)
		EDD_PlayerType PlayerType;

private:
	bool bIsUsed = false;
};