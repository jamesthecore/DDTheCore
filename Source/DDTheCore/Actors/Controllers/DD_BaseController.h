#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DD_BaseController.generated.h"

class UInputMappingContext;

UCLASS()
class ADD_BaseController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputMappingContext* InputMappingContext;
		
	virtual void BeginPlay() override;
};