#include "Components/DD_CompInteract.h"

#include "Actors/Characters/DD_CivilianCharacter.h"

UDD_CompInteract::UDD_CompInteract()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UDD_CompInteract::CanInteract() const
{
	return CurrentInteractedActor != nullptr;
}

AActor* UDD_CompInteract::GetInteractObject() const
{
	return CurrentInteractedActor;
}

void UDD_CompInteract::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = GetOwner();
}

void UDD_CompInteract::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const ADD_CivilianCharacter* CivilianCharacter = Cast<ADD_CivilianCharacter>(OwnerActor);
	if (!CivilianCharacter)
		return;
		
	const FVector& CameraDirection = CivilianCharacter->GetCameraDirection();
	const FVector& StartPosition = CivilianCharacter->GetActorLocation();
	const FVector EndPosition = StartPosition + Distance * CameraDirection;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(CivilianCharacter);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartPosition, EndPosition, ECC_GameTraceChannel2, Params);
	IInteractableInterface* InteractableInterface = Cast<IInteractableInterface>(HitResult.GetActor());
	if (InteractableInterface)
	{
		CurrentInteractedActor = HitResult.GetActor();
		InteractionDetected.Broadcast(CurrentInteractedActor);
	}
	else if (!InteractableInterface && CurrentInteractedActor)
	{
		CurrentInteractedActor = nullptr;
		InteractionDetected.Broadcast(nullptr);
	}
	
	DrawDebugLine(GetWorld(), StartPosition, EndPosition, FColor::Red, false, 1, 0, 1);
	
}
