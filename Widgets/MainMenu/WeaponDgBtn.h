// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Managers/DgContentManagers/WeaponScrollDgManager.h"

#include "WeaponDgBtn.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UWeaponDgBtn : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(int index, const FWeaponDgTableRow* weapon_dg);
};
