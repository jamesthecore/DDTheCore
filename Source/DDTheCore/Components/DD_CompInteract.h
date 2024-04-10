#pragma once

#include "CoreMinimal.h"
#include "DD_CompInteract.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionDetected, AActor*, InteractedActor);

UCLASS()
class UDD_CompInteract : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FInteractionDetected InteractionDetected;

	UDD_CompInteract();

	bool CanInteract() const;
	AActor* GetInteractObject() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Interact | Configuration")
		float Distance = 100.f;
	UPROPERTY()
		AActor* OwnerActor = nullptr;
	UPROPERTY()
		AActor* CurrentInteractedActor = nullptr;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};