
#pragma once

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
    bool m_bIsOpened=false;

    UMainCanvas(const FObjectInitializer& objInit);
protected:
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
    void OpenMainMenu();
    UFUNCTION(BlueprintCallable,Category="Menu")
    void CloseMainMenu();
    UFUNCTION(BlueprintCallable,Category="Menu")
    void OpenSkillMenu();
    UFUNCTION(BlueprintCallable,Category="Menu")
    void Interaction();
    UFUNCTION(BlueprintCallable,Category="Menu")
    void OnAttackPressStart();
    UFUNCTION(BlueprintCallable,Category="Menu")
    void OnAttackPressEnd();
    UFUNCTION(BlueprintCallable,Category="Menu")
    void OpenSetting();
    UFUNCTION(BlueprintCallable,Category="Menu")
    void CloseSetting();
    UFUNCTION(BlueprintCallable,Category="Menu")
    void TestOpenDungeon();
    UFUNCTION(BlueprintCallable,Category="Menu")
    void TestSaveAll();
    UFUNCTION(BlueprintCallable,Category="Menu")
    void DrinkPotion();
    UFUNCTION(BlueprintCallable,Category="Menu")
    void TestGoBackMenu();
public:
    void Init(ADiabloPlayerController * playerCon, APlayerDiabloCharacter * playerChar, UEquipmentSystem * equipment, UInventory * inven,TArray<UInventory*>* aryStorage);

    void ShowMonsterInfo(AUnitPawn* monInfo);

    void UpdateMonsterInfo(AUnitPawn* monInfo);
    
    void HideMonsterInfo();
    
    void UpdateExpGauge(float v);

    void UpdateHpBar();

    FORCEINLINE bool IsOpened() const
    {
        return m_bIsOpened;
    }

    
    void UpdateMinimap(UMaterialInterface* mapMat);
    
    
    void ShowBasicShopMenu(AShopKeeper* shopKeeper);

    void ShowStorageMenu();

    void ShowMinimap();
    
    void HideMinimap();

    void ShowSkillHotkeyPanel();

    void CloseSkillHotkeyPanel();

    UDiaShopPanel* GetShopPanelWidget();
    
    void OpenMapMenu(bool isCleared);

    void CloseMapMenu();
};
