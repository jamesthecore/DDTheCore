#pragma once

#include "CoreMinimal.h"
#include "DD_GeneratorSpawnerManager.generated.h"

class ADD_Generator;

UCLASS()
class ADD_GeneratorSpawnerManager : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
		TSubclassOf<ADD_Generator> GeneratorToSpawn;

	virtual void BeginPlay() override;

private:
	TArray<int32> GetRandomIndices(const uint8 MaxNumIndices, const int32 MaxIndexNumber) const;
};