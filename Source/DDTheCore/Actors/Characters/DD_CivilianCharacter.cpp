#include "Actors/Characters/DD_CivilianCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "UnrealNetwork.h"
#include "Actors/GameState/DD_GameStateMainGame.h"
#include "Components/CapsuleComponent.h"
#include "Components/DD_CompInteract.h"
#include "Components/DD_CompInteractable.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/DD_NetLibrary.h"

bool ADD_CivilianCharacter::IsInjured() const
{
	return CurrentState > EDD_CivilianState::FullHealth;
}

UDD_CompInteract* ADD_CivilianCharacter::GetCompInteract()
{
	return CompInteract;
}

ADD_CivilianCharacter::ADD_CivilianCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CompInteractable = CreateDefaultSubobject<UDD_CompInteractable>(TEXT("CompInteractable"));
	InteractCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractCollision"));
	CompInteract = CreateDefaultSubobject<UDD_CompInteract>(TEXT("CompInteract"));

	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);
	InteractCollision->SetupAttachment(RootComponent);

	SpringArmComponent->TargetArmLength = 400.f;
	SpringArmComponent->bUsePawnControlRotation = true;
}

void ADD_CivilianCharacter::RefreshCivilianState(bool bDamaged, int32 DamageAmount)
{
	if (!UDD_NetLibrary::IsServer(this))
	{
		UE_LOG(LogTemp, Error, TEXT("ADD_CivilianCharacter::RefreshCivilianStat should only be called from Server side"))
		return;
	}

	constexpr uint8 CriticalState = static_cast<uint8>(EDD_CivilianState::Critical);
	constexpr uint8 FullHealthState = static_cast<uint8>(EDD_CivilianState::FullHealth);
	uint8 State = static_cast<uint8>(CurrentState);

	if (bDamaged && State < CriticalState)
	{
		State += DamageAmount;
		if (State > CriticalState)
		{
			State = CriticalState;
		}
	}
	else if (!bDamaged && State > FullHealthState)
	{
		State += DamageAmount;
		if (State > FullHealthState)
		{
			State = FullHealthState;
		}
	}

	CurrentState = static_cast<EDD_CivilianState>(State);
}

UDD_CompInteractable* ADD_CivilianCharacter::GetCompInteractable()
{
	return CompInteractable;
}

FVector ADD_CivilianCharacter::GetCameraDirection() const
{
	return CameraComponent->GetComponentRotation().Vector();
}

void ADD_CivilianCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UDD_NetLibrary::IsServer(this) && CompInteractable)
	{
		CompInteractable->ServerStartInteraction.BindUObject(this, &ADD_CivilianCharacter::OnServerInteractionDetected);

		if (ADD_GameStateMainGame* GameState = Cast<ADD_GameStateMainGame>(UGameplayStatics::GetGameState(this)))
		{
			GameState->GeneratorServerStarted.AddUObject(this, &ADD_CivilianCharacter::GeneratorServerStarted);
		}
	}

	const AController* MyController = GetController();
	if (!MyController || !MyController->IsLocalController())
	{
		CompInteract->DestroyComponent();
		CompInteract = nullptr;
	}
}

void ADD_CivilianCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ADD_CivilianCharacter::Interact);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ADD_CivilianCharacter::InteractCanceled);
	}
}

void ADD_CivilianCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
	
}

void ADD_CivilianCharacter::Look(const FInputActionValue& Value)
{
	if (!Controller)
		return;

	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ADD_CivilianCharacter::Interact(const FInputActionValue& InputActionValue)
{
	IInteractableInterface* InteractObject = CompInteract ? Cast<IInteractableInterface>(CompInteract->GetInteractObject()) : nullptr;
	UDD_CompInteractable* OtherCompInteractable = InteractObject ? InteractObject->GetCompInteractable() : nullptr;
	if (OtherCompInteractable)
	{
		Server_Interact(OtherCompInteractable);
	}
}

void ADD_CivilianCharacter::InteractCanceled(const FInputActionValue& InputActionValue)
{
	IInteractableInterface* InteractObject = CompInteract ? Cast<IInteractableInterface>(CompInteract->GetInteractObject()) : nullptr;
	UDD_CompInteractable* OtherCompInteractable = InteractObject ? InteractObject->GetCompInteractable() : nullptr;
	if (OtherCompInteractable)
	{
		Server_CancelInteract(OtherCompInteractable);
	}
}

float ADD_CivilianCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!UDD_NetLibrary::IsServer(this))
	{
		UE_LOG(LogTemp, Error, TEXT("ADD_CivilianCharacter::TakeDamage should only be called from Server side"))
		return 0.f;
	}
	
	const float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	RefreshCivilianState(true, Damage);

	return Damage;
}

void ADD_CivilianCharacter::Server_Interact_Implementation(UDD_CompInteractable* OtherInteractable)
{
	if (OtherInteractable)
	{
		OtherInteractable->StartInteractionWithServer(this);
	}
}

void ADD_CivilianCharacter::Server_CancelInteract_Implementation(UDD_CompInteractable* OtherInteractable)
{
	if (OtherInteractable)
	{
		OtherInteractable->StopInteractionWithServer(this);
	}
}

void ADD_CivilianCharacter::GeneratorServerStarted(AActor* Actor)
{
	if (Actor != this)
		return;

	Multicast_PlayAnimation(Burpee);
}

void ADD_CivilianCharacter::Multicast_PlayAnimation_Implementation(UAnimMontage* AnimationToPlay)
{
	if (AnimationToPlay)
	{
		PlayAnimMontage(AnimationToPlay, 1.f);
	}
}

void ADD_CivilianCharacter::OnRep_CivilianState()
{
	FString s;
}

void ADD_CivilianCharacter::OnServerInteractionDetected(AActor* Interactor)
{
	CurrentState = EDD_CivilianState::FullHealth;
}

void ADD_CivilianCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADD_CivilianCharacter, CurrentState);
}
