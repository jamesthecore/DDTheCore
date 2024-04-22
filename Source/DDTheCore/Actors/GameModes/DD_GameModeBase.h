#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DD_GameModeBase.generated.h"

enum class EDD_PlayerType : uint8;

USTRUCT(BlueprintType)
struct FDD_Controller
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACharacter> PawnToSpawn;
};

UCLASS()
class ADD_GameModeBase : public AGameMode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Classes")
		TMap<EDD_PlayerType, FDD_Controller> CustomSpawnData;
	
	virtual const AActor* GetPlayerStart(EDD_PlayerType PlayerType) const;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	void OnServerUpdated(APlayerController* PlayerController);
};