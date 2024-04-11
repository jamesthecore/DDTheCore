#pragma once

#include "CoreMinimal.h"
#include "DD_CharacterBase.h"
#include "Interfaces/DD_DamageableInterface.h"
#include "Interfaces/DD_InteractableInterface.h"
#include "DD_CivilianCharacter.generated.h"

class UDD_CompInteract;
class UCameraComponent;
class USpringArmComponent;

UENUM(BlueprintType)
enum class EDD_CivilianState : uint8
{
	FullHealth,
	Hurt,
	Critical
};

UCLASS()
class ADD_CivilianCharacter : public ADD_CharacterBase, public IDamageableInterface, public IInteractableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
		bool IsInjured() const;
	UFUNCTION(BlueprintPure)
		UDD_CompInteract* GetCompInteract();

	ADD_CivilianCharacter();

	void RefreshCivilianState(bool bDamaged, int32 DamageAmount);

	virtual UDD_CompInteractable* GetCompInteractable() override;
	FVector GetCameraDirection() const;

protected:
	UPROPERTY(EditAnywhere)
		USpringArmComponent* SpringArmComponent = nullptr;
	UPROPERTY(EditAnywhere)
		UCameraComponent* CameraComponent = nullptr;
	UPROPERTY(EditDefaultsOnly)
		UDD_CompInteractable* CompInteractable = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCapsuleComponent* InteractCollision = nullptr;
	UPROPERTY(EditDefaultsOnly)
		UDD_CompInteract* CompInteract = nullptr;
	UPROPERTY(EditDefaultsOnly)
		UInputAction* InteractAction = nullptr;
	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* Burpee = nullptr;

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void Move(const FInputActionValue& Value) override;
	virtual void Look(const FInputActionValue& Value) override;
	void Interact(const FInputActionValue& InputActionValue);
	void InteractCanceled(const FInputActionValue& InputActionValue);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY(ReplicatedUsing=OnRep_CivilianState)
		EDD_CivilianState CurrentState = EDD_CivilianState::FullHealth;

	UFUNCTION(Server, Reliable)
		void Server_Interact(UDD_CompInteractable* OtherInteractable);
	UFUNCTION(Server, Reliable)
		void Server_CancelInteract(UDD_CompInteractable* OtherInteractable);
	UFUNCTION(NetMulticast, Unreliable)
		void Multicast_PlayAnimation(UAnimMontage* AnimationToPlay, const bool bStop);

	UFUNCTION()
		void OnRep_CivilianState();

	void OnServerInteractionDetected(AActor* Interactor);
	void GeneratorServerStarted(AActor* Actor);
	void GeneratorServerStop(AActor* Actor);
};