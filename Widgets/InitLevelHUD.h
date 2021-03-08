// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "SetNickname.h"
#include "Widgets/MyHUD.h"
#include "InitLevelHUD.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API AInitLevelHUD : public AMyHUD
{
	GENERATED_BODY()
public:
	AInitLevelHUD();

	
	UPROPERTY()
	TSubclassOf<USetNickname> m_ClassWidget;
	UPROPERTY()
	USetNickname* m_Canvas;

	virtual void BeginPlay() override;

	virtual void ReqeustPopupText(FText str) override;
};
