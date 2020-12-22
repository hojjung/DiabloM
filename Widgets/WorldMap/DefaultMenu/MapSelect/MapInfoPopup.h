// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Datas/DungeonDataTable.h"
#include "Widgets/CommonElement/ImageAndText.h"


#include "MapInfoPopup.generated.h"

class UDungeonManager;
/**
 * 
 */
UCLASS()
class DIABLOM_API UMapInfoPopup : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UImageAndText> m_ClassImgText;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_DgIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_DgName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnMiddle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextDgLevel;//dg level is not monster level
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnLeftMin;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnLeftDecrease;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRightMax;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRightIncrease;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClosePanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnEnterDg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* m_VerticalInfo1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* m_VerticalInfo2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* m_VerticalInfo3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImageAndText* m_MonsterLevel;//11~13
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImageAndText* m_DropItemLevel;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImageAndText* m_PlayerBuff;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImageAndText* m_PlayerDebuff;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImageAndText* m_MonsterBuff;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImageAndText* m_MonsterDebuff;
	//
	
protected:
	const FDungeonDataRow* m_CurrentDgData;
	UPROPERTY()
	TArray<UImageAndText*> m_AryImageTextCreated;
	UPROPERTY()
	UDungeonManager* m_DgManager;

	TSet<const FItemTypeHandle*> m_AryItemTypes;

	TSet<const FItemData*> m_AryItemData;

	TSet<const FUniqueEquipData*> m_AryUniqueItemData;

	FTextFormat m_FormatMonsterLevel;
	
	FTextFormat m_FormatItemLevel;
	
	int m_nCurrentDgLevel;

	int m_nMaxDgLevel;
	
protected:
	UImageAndText* CreateImageText(UTexture* texture,FText stringWant);
	
public:
	void Init();
	
	void SetMonsterAndItemLevel(const FDungeonDataRow* dg_data);

	void OpenPopup(const FDungeonDataRow* dg_data);

	


public:
	UFUNCTION()
    void ClosePopup();
	UFUNCTION()
	void DecreaseDgLv();
	UFUNCTION()
	void IncreaseDgLv();
	UFUNCTION()
	void DecreaseMin();
	UFUNCTION()
    void IncreaseMax();
	UFUNCTION()
	void EnterDungeon();
	
};

