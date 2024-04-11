#include "Actors/DD_Generator.h"

#include "UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "Components/DD_CheckSkillComponent.h"
#include "Components/DD_CompInteractable.h"
#include "GameState/DD_GameStateMainGame.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/DD_NetLibrary.h"

float ADD_Generator::GetCurrentHealth() const
{
	return CurrentHealth;
}

float ADD_Generator::GetMaxHealth() const
{
	return MaxHealth;
}

ADD_Generator::ADD_Generator()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	InteractBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBoxComponent"));
	CompInteractable = CreateDefaultSubobject<UDD_CompInteractable>(TEXT("CompInteractable"));
	CheckSkillComponent = CreateDefaultSubobject<UDD_CheckSkillComponent>(TEXT("CheckSkillComponent"));

	RootComponent = MeshComponent;
	InteractBoxComponent->SetupAttachment(MeshComponent);
}

UDD_CompInteractable* ADD_Generator::GetCompInteractable()
{
	return CompInteractable;
}

void ADD_Generator::OnRep_CurrentHealth()
{
	FString s;
}

void ADD_Generator::BeginPlay()
{
	Super::BeginPlay();

	if (UDD_NetLibrary::IsServer(this))
	{
		CompInteractable->ServerStartInteraction.BindUObject(this, &ADD_Generator::OnServerStartInteraction);
		CompInteractable->ServerStopInteraction.BindUObject(this, &ADD_Generator::OnServerStopInteraction);
	}
}

void ADD_Generator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (const ADD_GameStateMainGame* MyGameState = Cast<ADD_GameStateMainGame>(UGameplayStatics::GetGameState(this)))
	{
		MyGameState->GeneratorServerStop.Broadcast(nullptr);
	}

	GetWorldTimerManager().ClearTimer(HealthTimer);

	Super::EndPlay(EndPlayReason);
}

void ADD_Generator::OnServerStartInteraction(AActor* Interactor)
{
	Interactions++;
	if (const ADD_GameStateMainGame* MyGameState = Cast<ADD_GameStateMainGame>(UGameplayStatics::GetGameState(this)))
	{
		MyGameState->GeneratorServerStarted.Broadcast(Interactor);
	}

	GetWorldTimerManager().SetTimer(HealthTimer, this, &ADD_Generator::OnHealthTimer, 2.f);
	constexpr bool bSkillCheckAvailable = true;
	CheckSkillComponent->SetSkillCheckAvailable(bSkillCheckAvailable);
}

void ADD_Generator::OnServerStopInteraction(AActor* Interactor)
{
	if (const ADD_GameStateMainGame* MyGameState = Cast<ADD_GameStateMainGame>(UGameplayStatics::GetGameState(this)))
	{
		MyGameState->GeneratorServerStop.Broadcast(Interactor);
	}

	GetWorldTimerManager().ClearTimer(HealthTimer);
	if (Interactions == 0)
	{
		constexpr bool bSkillCheckAvailable = false;
		CheckSkillComponent->SetSkillCheckAvailable(bSkillCheckAvailable);
	}
	
	--Interactions;
}

void ADD_Generator::OnHealthTimer()
{
	if (!UDD_NetLibrary::IsServer(this))
	{
		UE_LOG(LogTemp, Error, TEXT("ADD_Generator::OnHealthTimer should only be called in server side."));
		return;
	}

	CurrentHealth += HealthRatio;
	if (CurrentHealth >= MaxHealth)
	{
		Destroy();
		return;
	}

	GetWorldTimerManager().SetTimer(HealthTimer, this, &ADD_Generator::OnHealthTimer, 2.f);
}

void ADD_Generator::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADD_Generator, CurrentHealth);
}
