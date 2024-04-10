#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DD_BaseLibrary.generated.h"

class UDD_CompInteractable;

UCLASS()
class UDD_BaseLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
		static UDD_CompInteractable* GetCompInteractable(AActor* ActorToCheck);
};