#pragma once

#include "CoreMinimal.h"
#include "DD_CheckSkillComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillCheck, bool, bSuccess);

UCLASS()
class UDD_CheckSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDD_CheckSkillComponent();
	void SetSkillCheckAvailable(bool bIsAvailable);

protected:
	UPROPERTY(BlueprintAssignable)
		FSkillCheck SkillCheck;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FTimerHandle SkillCheckTimer;
	bool bSkillCheckAvailable = false;

	void OnSkillCheckTimer();
	bool CanSendSkillCheck() const;
};