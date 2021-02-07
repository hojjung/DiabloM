
#pragma once

#include "BackgroundBlur.h"
#include "DiabloM.h"
#include "TechTreeWidget.h"
#include "Blueprint/UserWidget.h"
#include "MapSelect/MapSelectPanel.h"
#include "Shop/DiaShopPanel.h"


#include "MainCanvas.generated.h"

class AShopKeeper;
class UMaterialProgressBar;
class UInventory;
class UEquipmentSystem;
class APlayerDiabloCharacter;
class ADiabloPlayerController;
class UDefaultMenu;
class AUnitPawn;
class UDiaMonsterInfo;
/*
 *
 * 
 */
 class UPlayerStatusBar;
UCLASS()
class DIABLOM_API UMainCanvas : public UUserWidget
{
	GENERATED_BODY()

public:
    UMainCanvas(const FObjectInitializer& objInit);

    DECLARE_MULTICAST_DELEGATE_OneParam(FOnWidgetOpenClose,bool);

    FOnWidgetOpenClose m_OnWidgetOpenClose;
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
    UBackgroundBlur* m_BGBlur;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
    UButton* m_BtnCloseMenu;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
    UDefaultMenu* m_MainMenu;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
    UPlayerStatusBar* m_PlayerStatusBar;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
    UMapSelectPanel* m_MapSelect;
    
    //PlayerStatus
protected:
    UPROPERTY()
    ADiabloPlayerController* m_PlayerCon;
    UPROPERTY()
    APlayerDiabloCharacter* m_PlayerPawn;
    UPROPERTY()
    UEquipmentSystem* m_EquipSys;
    UPROPERTY()
    UInventory* m_Inven;

    TArray<UInventory*>* m_Storage;

    FDelegateHandle m_MonUpdateHandle;
    
    FTextFormat m_HpFormat;
public:
    UFUNCTION(BlueprintCallable,Category="Menu")
    void Interaction();
    UFUNCTION(BlueprintCallable,Category="Menu")
    void DrinkPotion();
    
public:
    UFUNCTION(BlueprintCallable,Category="Menu")
    void TestOpenDungeon();
    UFUNCTION(BlueprintCallable,Category="Menu")
    void TestSaveAll();
    UFUNCTION(BlueprintCallable,Category="Menu")
    void TestGoBackMenu();

protected:
    void OnOpenClosePlayerHUD(bool isOpen);

    void OnMonsterFocused(AUnitPawn* monInfo);
    
public:
    void Init(ADiabloPlayerController * playerCon, APlayerDiabloCharacter * playerChar, UEquipmentSystem * equipment, UInventory * inven,TArray<UInventory*>* aryStorage);
    //
    void OpenMonsterInfo(AUnitPawn* monInfo);

    void UpdateMonsterInfo(AUnitPawn* monInfo);
    
    void CloseMonsterInfo();
    //
    void UpdateExpGauge(float v);

    void UpdateHpBar(int, FItemInstance&);
    //
    void OpenMinimap();

    void UpdateMinimap(UMaterialInterface* mapMat);
    
    void CloseMinimap();
    //
    UFUNCTION()
    void OpenMainMenu();
    UFUNCTION()
    void CloseMainMenu();
    //
    void OpenBasicShopMenu(AShopKeeper* shopKeeper);

    void CloseBasicShopMenu();
    //
    void OpenStorageMenu();

    void CloseStorageMenu();
    //
    void OpenSkillHotkeyPanel();

    void CloseSkillHotkeyPanel();
    //
    void OpenMapMenu(bool isCleared);

    void CloseMapMenu();
    //
    UFUNCTION()
    void OpenSkillMenu();
    
    void CloseSkillMenu();
    //
    UFUNCTION()
    void OpenSetting();
    
    void CloseSetting();

public:
    FORCEINLINE UDefaultMenu* GetMainMenu()
    {
        return m_MainMenu;
    }
};
