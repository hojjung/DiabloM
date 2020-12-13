// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystem/Components/PlayerDiabloAbilitySystemComp.h"
#include "Blueprint/UserWidget.h"
#include "DiaTalentPanel.generated.h"

class UDefaultMenu;
/**
 * 
 */
UCLASS()
class DIABLOM_API UDiaTalentPanel : public UUserWidget
{
	GENERATED_BODY()
    
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
    UTextBlock* m_TextPointAvailable;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
    UTextBlock* m_TextPointTotalSpent;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
    UButton* m_ResetButton;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
    UTechTreeWidget* m_WarriorTechTreeWidget;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
    UTechTreeWidget* m_HunterTechTreeWidget;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
    UTechTreeWidget* m_MageTechTreeWidget;
protected:
    UPROPERTY()
    UTechTreeWidget* m_SelectedTalentTechtree;
    UPROPERTY()
    UPlayerDiabloAbilitySystemComp* m_PlayerTalentComp;

    FTextFormat m_FormatTalentPoint;

    FTextFormat m_FormatTotalTalentPoint;
    
public:
    void Init(UPlayerDiabloAbilitySystemComp* player);
    TArray<UTechNodeWidget*> GetTechNodeWidgets();

protected:
    UFUNCTION()
    void UpdateAvailablePoint();
    UFUNCTION()
    void UpdateTotalPoint();
    UFUNCTION()
    void ResetTalentPoint();


    friend  UDefaultMenu;
};
