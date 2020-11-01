// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "DungeonMiniMap.h"
#include "Blueprint/UserWidget.h"
#include "MinimapWidget.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UMinimapWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	//UDungeonMiniMap
public:
	void UpdateMinimap(UMaterialInterface* material_interface);
};
