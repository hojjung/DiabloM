// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Managers/DgContentManagers/PetDgManager.h"

#include "PetDgBtn.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UPetDgBtn : public UUserWidget
{
	GENERATED_BODY()

public:

	void Init(int index, const FPetDgTableRow* petDg);
};
