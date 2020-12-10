// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "SkillLearnButton.h"
#include "AbilitySystem/Components/PlayerDiabloAbilitySystemComp.h"
#include "Blueprint/UserWidget.h"
#include "DiaSkillPanel.generated.h"

class UDefaultMenu;

UCLASS()
class DIABLOM_API UDiaSkillPanel : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextBaseSkillName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextPowerSkillName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextDefensiveSkillName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextSpecialSkillName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextMasterySkillName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextUltimateSkillName;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UHorizontalBox* m_BaseSkillBelt;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UHorizontalBox* m_PowerSkillBelt;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UHorizontalBox* m_DefenseSkillBelt;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UHorizontalBox* m_SpeicalSkillBelt;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UHorizontalBox* m_MasterySkillBelt;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UHorizontalBox* m_UltimateSkillBelt;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextPointAvailable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextPointTotalSpent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UButton* m_ResetButton;
	
protected:
	UPROPERTY()
	UPlayerDiabloAbilitySystemComp* m_PlayerSkillComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<USkillLearnButton> m_ClassSkillLearnButton;

	FTextFormat m_FormatSkillPoint;

	FTextFormat m_FormatTotalSkillPoint;
	
	UPROPERTY()
	TArray<USkillLearnButton*> m_AryAllSkillLearnButton;

public:
	
	void Init(UPlayerDiabloAbilitySystemComp* playerSkillComp);

private:
	USkillLearnButton* CreateSkillButton(FSkillDataSpec& skillSpec,UPlayerDiabloAbilitySystemComp* compDi);

public:
	UFUNCTION()
	void ResetSkillPoint();
	UFUNCTION()
	void UpdateAvailablePoint();
	UFUNCTION()
    void UpdateTotalPoint();

	void UpdateAllWidgetWrap(FSkillDataSpec* notUse=nullptr);
	
	void UpdateAllWidget(FSkillDataSpec* notUse=nullptr,int notUseIndex=-1);


	FORCEINLINE TArray<USkillLearnButton*> GetAllSkillLearnBtn()
	{
		return m_AryAllSkillLearnButton;
	}

	friend UDefaultMenu;
};
