#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DD_GameModeMainGame.generated.h"

enum class EDD_PlayerType : uint8;

USTRUCT(BlueprintType)
struct FDD_Controller
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ACharacter> PawnToSpawn;
};

UCLASS()
class ADD_GameModeMainGame : public AGameMode
{
	GENERATED_BODY()

public:
	uint8 GetMaxNumGenerators() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Classes")
		TMap<EDD_PlayerType, FDD_Controller> CustomSpawnData;
	UPROPERTY(EditAnywhere, Category = "DD | Configuration")
		uint8 MaxNumGenerators = 0;

	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	//TODO. To be removed.
	bool bAssassinSet = false;
};