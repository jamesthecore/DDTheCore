#pragma once

#include "CoreMinimal.h"
#include "Interfaces/DD_InteractableInterface.h"
#include "DD_Generator.generated.h"

class UDD_CheckSkillComponent;
class UBoxComponent;

UCLASS()
class ADD_Generator : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
		float GetCurrentHealth() const;
	UFUNCTION(BlueprintPure)
		float GetMaxHealth() const;

	ADD_Generator();

	virtual UDD_CompInteractable* GetCompInteractable() override;

protected:
	UPROPERTY(EditDefaultsOnly)
		float MaxHealth = 10.f;
	UPROPERTY(EditDefaultsOnly)
		float HealthRatio = 0.2f;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MeshComponent = nullptr;
	UPROPERTY(EditAnywhere)
		UBoxComponent* InteractBoxComponent = nullptr;
	UPROPERTY(EditAnywhere)
		UDD_CompInteractable* CompInteractable = nullptr;
	UPROPERTY(EditDefaultsOnly)
		UDD_CheckSkillComponent* CheckSkillComponent = nullptr;

	UPROPERTY(ReplicatedUsing=OnRep_CurrentHealth)
		float CurrentHealth = 0.f;

	uint8 Interactions = 0;

	FTimerHandle HealthTimer;

	UFUNCTION()
		void OnRep_CurrentHealth();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void OnServerStartInteraction(AActor* Interactor);
	void OnServerStopInteraction(AActor* Interactor);
	void OnHealthTimer();
};