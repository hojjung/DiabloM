
#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "MainCanvas.generated.h"

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
 
UCLASS()
class DIABLOM_API UMainCanvas : public UUserWidget
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
    UButton* m_InvenButton;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
    UButton* m_SettingButton;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
    UButton* m_AttackButton;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
    UButton* m_InteractButton;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
    UDefaultMenu* m_MainMenu;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
    UDiaMonsterInfo* m_DiaMonInfo;
protected:
    UPROPERTY()
    ADiabloPlayerController* m_PlayerCon;
    UPROPERTY()
    APlayerDiabloCharacter* m_PlayerPawn;
    UPROPERTY()
    UEquipmentSystem* m_EquipSys;
    UPROPERTY()
    UInventory* m_Inven;

    FDelegateHandle m_MonUpdateHandle;
public:
    UFUNCTION(BlueprintCallable,Category="Menu")
    void OpenMainMenu();
    UFUNCTION(BlueprintCallable,Category="Menu")
    void CloseMainMenu();
    UFUNCTION(BlueprintCallable,Category="Menu")
    void Interaction();
    UFUNCTION(BlueprintCallable,Category="Menu")
    void Attack();
    UFUNCTION(BlueprintCallable,Category="Menu")
    void OpenSetting();
    UFUNCTION(BlueprintCallable,Category="Menu")
    void CloseSetting();
    
public:
    void Init(ADiabloPlayerController * playerCon, APlayerDiabloCharacter * playerChar, UEquipmentSystem * equipment, UInventory * inven);

    void ShowMonsterInfo(AUnitPawn* monInfo);

    void UpdateMonsterInfo(AUnitPawn* monInfo);
    
    void HideMonsterInfo();
};
