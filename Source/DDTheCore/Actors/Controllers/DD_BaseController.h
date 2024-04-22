#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DD_BaseController.generated.h"

struct FDD_SharedPlayerInformation;
class UInputMappingContext;

DECLARE_MULTICAST_DELEGATE_OneParam(FServerSharedInformationUpdated, APlayerController*)

UCLASS()
class ADD_BaseController : public APlayerController
{
	GENERATED_BODY()

public:
	FServerSharedInformationUpdated InformationUpdated;

	UFUNCTION(Client, Reliable)
		void Client_UpdateOnlineGameInstanceInformation();
	UFUNCTION(Server, Reliable)
		void Server_UpdateOnlineGameInstanceInformation(const FDD_SharedPlayerInformation& ClientInformation);

	bool bInformationShared = false;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputMappingContext* InputMappingContext;
		
	virtual void BeginPlay() override;
};