#include "Actors/GameModes/DD_GameModeBase.h"
#include "Actors/Controllers/DD_BaseController.h"
#include "GameFramework/Character.h"
#include "GameInstances/DD_OnlineGameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"

const AActor* ADD_GameModeBase::GetPlayerStart(EDD_PlayerType PlayerType) const
{
	return nullptr;
}

void ADD_GameModeBase::PostLogin(APlayerController* NewPlayer)
{
	ADD_BaseController* BaseController = Cast<ADD_BaseController>(NewPlayer);
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	UDD_OnlineGameInstanceSubsystem* OnlineGameInstanceSubsystem = GameInstance ? GameInstance->GetSubsystem<UDD_OnlineGameInstanceSubsystem>() : nullptr;

	if (!BaseController || !OnlineGameInstanceSubsystem)
	{
		Super::PostLogin(NewPlayer);
		return;
	}

	BaseController->InformationUpdated.AddUObject(this, &ADD_GameModeBase::OnServerUpdated);
	BaseController->Client_UpdateOnlineGameInstanceInformation();
}

void ADD_GameModeBase::OnServerUpdated(APlayerController* PlayerController)
{
	ADD_BaseController* BaseController = Cast<ADD_BaseController>(PlayerController);
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	UDD_OnlineGameInstanceSubsystem* OnlineGameInstanceSubsystem = GameInstance ? GameInstance->GetSubsystem<UDD_OnlineGameInstanceSubsystem>() : nullptr;

	if (!BaseController || !OnlineGameInstanceSubsystem)
		return;

	const EDD_PlayerType TypeToSpawn = OnlineGameInstanceSubsystem->GetSharedPlayerInformation().PlayerType;
	const AActor* FoundStart = GetPlayerStart(TypeToSpawn);
	if (!FoundStart)
	{
		UGameplayStatics::OpenLevel(this, "MainMenu");
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (ACharacter* SpawnedCharacter = GetWorld()->SpawnActor<ACharacter>(CustomSpawnData[TypeToSpawn].PawnToSpawn, FoundStart->GetActorLocation(), FoundStart->GetActorRotation(), Params))
	{
		UE_LOG(LogTemp, Error, TEXT("Spawned"));
		APawn* CurrentPawn = PlayerController->GetPawn();
		PlayerController->UnPossess();

		if (CurrentPawn)
		{
			CurrentPawn->Destroy();
		}

		PlayerController->Possess(SpawnedCharacter);

		if (ADD_PlayerStateBase* PlayerStateBase = PlayerController->GetPlayerState<ADD_PlayerStateBase>())
		{
			PlayerStateBase->SetPlayerType(TypeToSpawn);
		}
	}

	Super::PostLogin(PlayerController);
}
