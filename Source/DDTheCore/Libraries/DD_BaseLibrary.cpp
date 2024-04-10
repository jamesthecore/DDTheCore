#include "Libraries/DD_BaseLibrary.h"
#include "Interfaces/DD_InteractableInterface.h"

UDD_CompInteractable* UDD_BaseLibrary::GetCompInteractable(AActor* ActorToCheck)
{
	IInteractableInterface* InteractableInterface = Cast<IInteractableInterface>(ActorToCheck);
	return InteractableInterface ? InteractableInterface->GetCompInteractable() : nullptr;
}
