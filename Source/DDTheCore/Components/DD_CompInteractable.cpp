#include "Components/DD_CompInteractable.h"

const FDD_InteractInformation& UDD_CompInteractable::GetInteractInformation() const
{
	return InteractInformation;
}

void UDD_CompInteractable::StartInteractionWithServer(AActor* Interactor)
{
	ServerStartInteraction.ExecuteIfBound(Interactor);
}

void UDD_CompInteractable::StopInteractionWithServer(AActor* Interactor)
{
	ServerStopInteraction.ExecuteIfBound(Interactor);
}

void UDD_CompInteractable::StartInteractionWithClient(AActor* Interactor)
{
	ClientStartInteraction.ExecuteIfBound(Interactor);
}

void UDD_CompInteractable::StopInteractionWithClient(AActor* Interactor)
{
	ClientStopInteraction.ExecuteIfBound(Interactor);
}