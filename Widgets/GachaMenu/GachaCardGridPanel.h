
#pragma once

#include "DiabloM.h"
#include "GachaCard.h"
#include "UniformGridPanel.h"
#include "Blueprint/UserWidget.h"
#include "Managers/GachaManager.h"
#include "Managers/ShopManager.h"


#include "GachaCardGridPanel.generated.h"

UCLASS()
class DIABLOM_API UGachaCardGridPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	TWeakObjectPtr<UShopManager> m_ShopManager;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGachaCard> m_ClassGachaCard;
	UPROPERTY()
	TArray<UGachaCard*> m_AryGachaCard;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnConfirm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UUniformGridPanel* m_Grid;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnGachaOneTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnGachaElevenTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnGachaFiftyTime;

	ERollItemType m_CurrentType;

protected:
	void HideCardGrid();
	
public:
	virtual void NativeOnInitialized() override;
	
	void SetRollGachaData(ERollItemType gachaTable);

public:
	UFUNCTION()
	void ClosePanel();
	UFUNCTION()
    void RollGachaOneTime();
	UFUNCTION()
    void RollGachaElevenTime();
	UFUNCTION()
    void RollGachaFiftyTime();


	FORCEINLINE ERollItemType GetCurrentType ()
	{
		return 	m_CurrentType;
	}
};
