#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DD_CharacterBase.generated.h"

class UInputMappingContext;
struct FInputActionValue;
class UInputAction;

UCLASS()
class ADD_CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable)
		void Server_AddTag(const FName& Tag);
	UFUNCTION(Server, Reliable)
		void Server_RemoveTag(const FName& Tag);
	UFUNCTION(NetMulticast, Unreliable)
		void Multicast_AddTag(const FName& Tag);
	UFUNCTION(NetMulticast, Unreliable)
		void Multicast_RemoveTag(const FName& Tag);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* LookAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
		UInputAction* MoveAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
		UInputMappingContext* DefaultMappingContext = nullptr;

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual void Move(const FInputActionValue& Value) {};
	virtual void Look(const FInputActionValue& Value) {};
};