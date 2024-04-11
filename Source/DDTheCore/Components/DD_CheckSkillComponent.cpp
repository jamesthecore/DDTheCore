#include "Components/DD_CheckSkillComponent.h"

UDD_CheckSkillComponent::UDD_CheckSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDD_CheckSkillComponent::SetSkillCheckAvailable(bool bIsAvailable)
{
	bSkillCheckAvailable = bIsAvailable;
}

void UDD_CheckSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bSkillCheckAvailable)
		return;

	if (CanSendSkillCheck())
	{
		GetOwner()->GetWorldTimerManager().SetTimer(SkillCheckTimer, this, &UDD_CheckSkillComponent::OnSkillCheckTimer, 5.f);
	}
}

void UDD_CheckSkillComponent::OnSkillCheckTimer()
{
	constexpr bool bHasCompletedSkillCheck = false;
	SkillCheck.Broadcast(bHasCompletedSkillCheck);
}

bool UDD_CheckSkillComponent::CanSendSkillCheck() const
{
	return false;
}