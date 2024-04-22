#pragma once

#include "CoreMinimal.h"
#include "DD_GameModeBase.h"
#include "DD_GameModeMainGame.generated.h"

enum class EDD_PlayerType : uint8;

UCLASS()
class ADD_GameModeMainGame : public ADD_GameModeBase
{
	GENERATED_BODY()

public:
	uint8 GetMaxNumGenerators() const;

protected:
	UPROPERTY(EditAnywhere, Category = "DD | Configuration")
		uint8 MaxNumGenerators = 0;

	virtual const AActor* GetPlayerStart(EDD_PlayerType PlayerType) const override;
};