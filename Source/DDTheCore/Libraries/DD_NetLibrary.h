#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DD_NetLibrary.generated.h"

UCLASS()
class UDD_NetLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
		static bool IsServer(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure)
		static bool IsClient(const UObject* WorldContextObject);
};