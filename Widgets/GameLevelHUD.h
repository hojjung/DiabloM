// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "DiabloM.h"
#include "Widgets/MyHUD.h"
#include "GameLevelHUD.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API AGameLevelHUD : public AMyHUD
{
public:
	AGameLevelHUD();
	
	GENERATED_BODY()
	TSubclassOf<UMainCanvas> m_ClassWidget;

	UPROPERTY()
	UMainCanvas* m_Canvas;
	
	virtual void BeginPlay() override;

	virtual void ReqeustPopupText(FText str) override;

	virtual void OnPressBackkey() override;

	void ShowOfflineGoldWindow(BigInt gold);

	void ShowTouchBan(float secWant);
};
