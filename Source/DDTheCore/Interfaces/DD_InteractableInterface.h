#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DD_InteractableInterface.generated.h"

class UDD_CompInteractable;

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class IInteractableInterface
{
	GENERATED_BODY()

public:
	virtual UDD_CompInteractable* GetCompInteractable() { return nullptr; }
};
