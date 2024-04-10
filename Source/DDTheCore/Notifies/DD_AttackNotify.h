#pragma once

#include "CoreMinimal.h"
#include "DD_AttackNotify.generated.h"

UCLASS()
class UDD_AttackNotify : public UAnimNotify
{
	GENERATED_BODY()

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};