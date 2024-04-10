#pragma once

#include "CoreMinimal.h"
#include "DD_CharacterBase.h"
#include "DD_AssassinCharacter.generated.h"

class USphereComponent;
enum class EDD_PlayerType : uint8;
class UInputMappingContext;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

UCLASS()
class ADD_AssassinCharacter : public ADD_CharacterBase
{
	GENERATED_BODY()

public:
	UFUNCTION()
		void OnAttackComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	ADD_AssassinCharacter();
	void ActivateAttack();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void Move(const FInputActionValue& Value) override;
	virtual void Look(const FInputActionValue& Value) override;

	void Shoot(const FInputActionValue& Value);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
		UInputAction* ShootAction = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent = nullptr;
	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* ShootMontage = nullptr;
	UPROPERTY(EditDefaultsOnly)
		USphereComponent* AttackSphere = nullptr;

	UPROPERTY(Replicated)
		TArray<AActor*> AlreadyDamaged;

	UFUNCTION(Server, Reliable)
		void Server_AddScore(EDD_PlayerType Type);
	UFUNCTION(Server, Unreliable)
		void Server_PlayAnimation(UAnimMontage* AnimToPlay);
	UFUNCTION(Server, Reliable)
		void Server_ApplyDamage(AActor* DamagedActor);
	UFUNCTION(NetMulticast, Unreliable)
		void Multicast_PlayAnimation(UAnimMontage* AnimToPlay);

	bool bIsAttackActive = false;
	
};