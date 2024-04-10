#include "Notifies/DD_AttackNotify.h"
#include "Actors/Characters/DD_AssassinCharacter.h"

void UDD_AttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ADD_AssassinCharacter* OwnerCharacter = Cast<ADD_AssassinCharacter>(MeshComp->GetOwner());
	const APlayerController* OwnerController = OwnerCharacter ? Cast<APlayerController>(OwnerCharacter->GetController()) : nullptr;
	if (OwnerController && OwnerController->IsLocalController())
	{
		OwnerCharacter->ActivateAttack();
	}
}
