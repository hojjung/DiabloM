#pragma once

#include "DiabloM.h"
#include "GachaInfoElement.h"
#include "ScrollBox.h"
#include "Blueprint/UserWidget.h"
#include "Datas/GachaDataTable.h"
#include "GachaInfoPanel.generated.h"

UCLASS()
class DIABLOM_API UGachaInfoPanel : public UUserWidget
{
	GENERATED_BODY()
	
	//도구 이름 과 확률
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_Verti;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGachaInfoElement> m_ClassInfoElement;
	UPROPERTY()
	TArray<UGachaInfoElement*> m_AryGachaInfoElement;
	
public:
	virtual void NativeOnInitialized() override;
	
	void OpenGachaInfo(TArray<FGachaTableRow*>& aryTableRow,float totalPercent);

	UFUNCTION()
	void CloseGachaPanel();
};
