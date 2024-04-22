#include "Actors/Controllers/DD_BaseController.h"
#include "EnhancedInputSubsystems.h"
#include "GameInstances/DD_OnlineGameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"

void ADD_BaseController::Client_UpdateOnlineGameInstanceInformation_Implementation()
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	UDD_OnlineGameInstanceSubsystem* OnlineGameInstanceSubsystem = GameInstance ? GameInstance->GetSubsystem<UDD_OnlineGameInstanceSubsystem>() : nullptr;
	if (OnlineGameInstanceSubsystem)
	{
		Server_UpdateOnlineGameInstanceInformation(OnlineGameInstanceSubsystem->GetSharedPlayerInformation());
	}
}

void ADD_BaseController::Server_UpdateOnlineGameInstanceInformation_Implementation(const FDD_SharedPlayerInformation& ClientInformation)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	UDD_OnlineGameInstanceSubsystem* OnlineGameInstanceSubsystem = GameInstance ? GameInstance->GetSubsystem<UDD_OnlineGameInstanceSubsystem>() : nullptr;
	if (OnlineGameInstanceSubsystem)
	{
		OnlineGameInstanceSubsystem->SetSharedPlayerInformation(ClientInformation);
		InformationUpdated.Broadcast(this);
	}
}

void ADD_BaseController::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}
