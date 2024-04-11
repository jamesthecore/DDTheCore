#include "Actors/Characters/DD_CharacterBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Libraries/DD_NetLibrary.h"

void ADD_CharacterBase::Server_AddTag_Implementation(const FName& Tag)
{
	if (UDD_NetLibrary::IsServer(this))
	{
		Multicast_AddTag(Tag);
	}
}

void ADD_CharacterBase::Multicast_AddTag_Implementation(const FName& Tag)
{
	Tags.AddUnique(Tag);
}

void ADD_CharacterBase::Server_RemoveTag_Implementation(const FName& Tag)
{
	if (UDD_NetLibrary::IsServer(this))
	{
		Multicast_RemoveTag(Tag);
	}
}

void ADD_CharacterBase::Multicast_RemoveTag_Implementation(const FName& Tag)
{
	Tags.Remove(Tag);
}

void ADD_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = PlayerController ? ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()) : nullptr;
		
	if (Subsystem)
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ADD_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADD_CharacterBase::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADD_CharacterBase::Look);
	}
}