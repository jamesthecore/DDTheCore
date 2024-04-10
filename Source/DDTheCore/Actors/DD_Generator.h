#pragma once

#include "CoreMinimal.h"
#include "Interfaces/DD_InteractableInterface.h"
#include "DD_Generator.generated.h"

class UBoxComponent;

UCLASS()
class ADD_Generator : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	ADD_Generator();

	virtual UDD_CompInteractable* GetCompInteractable() override;

protected:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MeshComponent = nullptr;
	UPROPERTY(EditAnywhere)
		UBoxComponent* InteractBoxComponent = nullptr;
	UPROPERTY(EditAnywhere)
		UDD_CompInteractable* CompInteractable = nullptr;

	virtual void BeginPlay() override;

	void OnServerStartInteraction(AActor* Interactor);
	void OnServerStopInteraction(AActor* Interactor);
};