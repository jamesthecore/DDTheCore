#include "Actors/Characters/DD_AssassinCharacter.h"
#include "Actors/GameState/DD_GameStateMainGame.h"
#include "Actors/PlayerStates/DD_PlayerStateBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "DDTheCore.h"
#include "Engine/DamageEvents.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UnrealNetwork.h"
#include "Interfaces/DD_DamageableInterface.h"
#include "Kismet/GameplayStatics.h"

void ADD_AssassinCharacter::OnAttackComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const bool bCanReceiveDamage = OtherActor && Cast<IDamageableInterface>(OtherActor);
	if (!bCanReceiveDamage)
		return;

	Server_ApplyDamage(OtherActor);
}

ADD_AssassinCharacter::ADD_AssassinCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));

	GetMesh()->SetupAttachment(RootComponent);
	GetMesh()->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->SetupAttachment(GetMesh(), TEXT("headSocket"));

	AttackSphere->SetupAttachment(GetMesh(), TEXT("hand_lSocket"));
}

void ADD_AssassinCharacter::ActivateAttack()
{
	bIsAttackActive = !bIsAttackActive;

	if (!AttackSphere)
		return;

	if (bIsAttackActive)
	{
		AttackSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ADD_AssassinCharacter::OnAttackComponentBeginOverlap);
	}
	else
	{
		AttackSphere->OnComponentBeginOverlap.RemoveAll(this);
		AlreadyDamaged.Empty();
	}
}

void ADD_AssassinCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (!PlayerController->IsLocalController())
		{
			AttackSphere->DestroyComponent();
			AttackSphere = nullptr;
		}
	}
}

void ADD_AssassinCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ADD_AssassinCharacter::Shoot);
	}
}

void ADD_AssassinCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller)
		return;

	const FVector2D MovementVector = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void ADD_AssassinCharacter::Look(const FInputActionValue& Value)
{
	if (!Controller)
		return;

	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ADD_AssassinCharacter::Shoot(const FInputActionValue& Value)
{
	Server_PlayAnimation(ShootMontage);
	//Server_AddScore(EDD_PlayerType::Assassin);
}

void ADD_AssassinCharacter::Server_PlayAnimation_Implementation(UAnimMontage* AnimToPlay)
{
	if (AnimToPlay)
	{
		Multicast_PlayAnimation(AnimToPlay);
	}
}

void ADD_AssassinCharacter::Multicast_PlayAnimation_Implementation(UAnimMontage* AnimToPlay)
{
	if (AnimToPlay)
	{
		PlayAnimMontage(AnimToPlay);
	}
}

void ADD_AssassinCharacter::Server_ApplyDamage_Implementation(AActor* DamagedActor)
{
	if (!DamagedActor || AlreadyDamaged.Contains(DamagedActor))
		return;

	UGameplayStatics::ApplyDamage(DamagedActor, 1.f, GetController(), this, UDamageType::StaticClass());
	AlreadyDamaged.AddUnique(DamagedActor);
}

void ADD_AssassinCharacter::Server_AddScore_Implementation(EDD_PlayerType Type)
{
	if (!HasAuthority())
		return;

	const APlayerController* MyController = Cast<APlayerController>(GetController());
	ADD_PlayerStateBase* MyPlayerState = MyController ? MyController->GetPlayerState<ADD_PlayerStateBase>() : nullptr;
	if (MyPlayerState)
	{
		MyPlayerState->AddScore(10);
	}

	if (ADD_GameStateMainGame* GameStateMainGame = Cast<ADD_GameStateMainGame>(UGameplayStatics::GetGameState(this)))
	{
		GameStateMainGame->AddScore(Type);
	}
}

void ADD_AssassinCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADD_AssassinCharacter, AlreadyDamaged);
}