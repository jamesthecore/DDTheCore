#pragma once

#include "CoreMinimal.h"
#include "DD_CompInteractable.generated.h"

USTRUCT(Blueprintable)
struct FDD_InteractInformation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float InteractTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText InteractText;
};

DECLARE_DELEGATE_OneParam(FInteraction, AActor*)
UCLASS()
class UDD_CompInteractable : public UActorComponent
{
	GENERATED_BODY()

public:
	FInteraction ServerStartInteraction;
	FInteraction ServerStopInteraction;
	FInteraction ClientStartInteraction;
	FInteraction ClientStopInteraction;

	UFUNCTION(BlueprintPure)
		const FDD_InteractInformation& GetInteractInformation() const;

	void StartInteractionWithServer(AActor* Interactor);
	void StopInteractionWithServer(AActor* Interactor);
	void StartInteractionWithClient(AActor* Interactor);
	void StopInteractionWithClient(AActor* Interactor);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FDD_InteractInformation InteractInformation;
};