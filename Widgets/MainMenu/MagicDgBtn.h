// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Managers/DgContentManagers/MagicStoneDgManager.h"

#include "MagicDgBtn.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UMagicDgBtn : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(int index, const FMagicDgTableRow* magic_dg);
};
