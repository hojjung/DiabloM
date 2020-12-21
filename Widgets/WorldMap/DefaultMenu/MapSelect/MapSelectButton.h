// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "Button.h"
#include "Blueprint/UserWidget.h"
#include "Datas/DungeonDataTable.h"
#include "Widgets/CommonElement/ImageAndText.h"


#include "MapSelectButton.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UMapSelectButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMapSelectButton(const FObjectInitializer& objInit);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDgClick,const FDungeonDataRow*);
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    UButton* m_BtnOpenPopup;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImageAndText* m_ImgText;

	FOnDgClick m_OnDgClicked;

	const FDungeonDataRow* m_DgData;

protected:
	virtual void NativeOnInitialized() override;
	
public:
	UFUNCTION()
	void OnBttnClick();
	
	void InitButton(const FDungeonDataRow* dgData);
};
