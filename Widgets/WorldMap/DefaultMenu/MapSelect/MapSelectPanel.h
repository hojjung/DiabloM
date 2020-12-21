// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "Button.h"
#include "CanvasPanel.h"
#include "MapInfoPopup.h"
#include "MapSelectButton.h"
#include "WidgetSwitcher.h"
#include "Blueprint/UserWidget.h"
#include "Datas/DungeonDataTable.h"

#include "MapSelectPanel.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UMapSelectPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UMapSelectPanel(const FObjectInitializer& objInit);

	virtual void NativePreConstruct() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UMapSelectButton> m_ClassMapSelectBtn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDgDataHandle m_DefaultDgHandle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDgDataHandle m_DemonDgHandle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDgDataHandle m_BeastDgHandle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDgDataHandle m_UndeadDgHandle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDgDataHandle m_HordeDgHandle;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMapInfoPopup* m_MapInfoPopup;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetSwitcher* m_PanelSwitcher;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnMainDG;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRiteDG;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnEventDG;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClose;

	//m_BtnClose
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_MainDungeonPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMapSelectButton* m_BtnDefaultInf;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))//무한층으로하면 막힐때 찾아가기 애매함
	UMapSelectButton* m_BtnDemon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMapSelectButton* m_BtnBeast;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMapSelectButton* m_BtnUndead;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UMapSelectButton* m_BtnHorde;
	//
protected:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_RiteDungeonPanel;
	//
protected:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_EventDungeonPanel;
	
public:
	virtual void NativeOnInitialized() override;

	void InitDgButton(const FDungeonDataRow* dgData, UMapSelectButton* SlotCreated);

protected://need dg data
	UMapSelectButton* CreateDgBtn(const FDungeonDataRow* dgData);
	
	void OpenMapInfoPopup(const FDungeonDataRow* dgData);
	
public:
	UFUNCTION()
	void ShowMainDG();
	UFUNCTION()
    void ShowRiteDG();
	UFUNCTION()
    void ShowEventDG();
public:
	//일반 던전
	//얘네는 맵선택에 없어야정상?
	//제단에 불을피우면 맵선택에 생기게
	//특수던전
	UFUNCTION()
	void PowerDungeon();
	UFUNCTION()
	void KeyTypeDungeon();
	//이벤트던전 패널
	UFUNCTION()
	void EventGoldDungeon();
	UFUNCTION()
    void EventCowRoom();
	//랜덤던전도 있을것
	UFUNCTION()
	void CloseDgPanel();
};
