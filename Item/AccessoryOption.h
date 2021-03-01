#pragma once

#include "DiabloM.h"
#include "UObject/NoExportTypes.h"
#include "AccessoryOption.generated.h"

class UDiabloGameInstance;
class ADiabloPlayerController;
class APlayerDiabloCharacter;
/**
 * 
 */
UCLASS()
class DIABLOM_API UAccessoryOption : public UObject
{
	GENERATED_BODY()

public:
	virtual void ApplyAccessory(UDiabloGameInstance* gameInst,ADiabloPlayerController* plCon,APlayerDiabloCharacter* plChar){PRINTF("Apply Accessory");};

	virtual void RemoveAccessory(UDiabloGameInstance* gameInst,ADiabloPlayerController* plCon,APlayerDiabloCharacter* plChar){PRINTF("Remove Accessory");};
};
