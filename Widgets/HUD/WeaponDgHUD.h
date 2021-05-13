// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "WeaponDgCanvas.h"
#include "Widgets/HUD/MyHUD.h"
#include "WeaponDgHUD.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API AWeaponDgHUD : public AMyHUD
{
	GENERATED_BODY()

public:
	AWeaponDgHUD();
	
	UPROPERTY()
	TSubclassOf<UWeaponDgCanvas> m_ClassWidget;

	UPROPERTY()
	UWeaponDgCanvas* m_Canvas;
	
	virtual void BeginPlay() override;

	virtual void ReqeustPopupText(FText str) override;
};
