#include "Actors/DD_Generator.h"
#include "Components/BoxComponent.h"
#include "Components/DD_CompInteractable.h"
#include "GameState/DD_GameStateMainGame.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/DD_NetLibrary.h"

ADD_Generator::ADD_Generator()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	InteractBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBoxComponent"));
	CompInteractable = CreateDefaultSubobject<UDD_CompInteractable>(TEXT("CompInteractable"));

	RootComponent = MeshComponent;
	InteractBoxComponent->SetupAttachment(MeshComponent);
}

UDD_CompInteractable* ADD_Generator::GetCompInteractable()
{
	return CompInteractable;
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

void ADD_Generator::OnServerStartInteraction(AActor* Interactor)
{
	if (const ADD_GameStateMainGame* MyGameState = Cast<ADD_GameStateMainGame>(UGameplayStatics::GetGameState(this)))
	{
		MyGameState->GeneratorServerStarted.Broadcast(Interactor);
	}
}

void ADD_Generator::OnServerStopInteraction(AActor* Interactor)
{
}
