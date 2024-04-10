#include "Actors/DD_GeneratorSpawnerManager.h"

#include "DD_Generator.h"
#include "DD_GeneratorStart.h"
#include "GameModes/DD_GameModeMainGame.h"
#include "Kismet/GameplayStatics.h"

void ADD_GeneratorSpawnerManager::BeginPlay()
{
	Super::BeginPlay();

	// If game mode exists it means we're in the server instance.
	if (const ADD_GameModeMainGame* GameMode = Cast<ADD_GameModeMainGame>(UGameplayStatics::GetGameMode(this)))
	{
		const uint8 MaxNumGenerators = GameMode->GetMaxNumGenerators();
		TArray<AActor*> Generators;
		UGameplayStatics::GetAllActorsOfClass(this, ADD_GeneratorStart::StaticClass(), Generators);
		const TArray<int32> RandomIndices = GetRandomIndices(MaxNumGenerators, Generators.Num());
		for (const int32 Index : RandomIndices)
		{
			if (const AActor* Generator = Generators[Index])
			{
				if (ADD_Generator* SpawnedGenerator = GetWorld()->SpawnActor<ADD_Generator>(GeneratorToSpawn, Generator->GetActorTransform()))
				{
					FString s;
				}
			}
			
		}
	}
}

TArray<int32> ADD_GeneratorSpawnerManager::GetRandomIndices(const uint8 MaxNumIndices, const int32 MaxIndexNumber) const
{
	TArray<int32> RandomIndices;
	for (uint8 Index = 0; Index < MaxNumIndices;)
	{
		const int32 RandomIndex = FMath::RandRange(0, MaxIndexNumber - 1);
		if (!RandomIndices.Contains(RandomIndex))
		{
			++Index;
			RandomIndices.AddUnique(RandomIndex);
		}
	}
	return RandomIndices;
}