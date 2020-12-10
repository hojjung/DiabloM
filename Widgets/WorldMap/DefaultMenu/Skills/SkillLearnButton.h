// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"


#include "CheckBox.h"
#include "DiaDragDropSkill.h"
#include "Blueprint/UserWidget.h"
#include "Datas/SkillDataTable.h"
#include "Widgets/CommonElement/CooldownProgress.h"


#include "SkillLearnButton.generated.h"

class UPlayerDiabloAbilitySystemComp;

UCLASS()
class DIABLOM_API USkillLearnButton : public UUserWidget
{
	GENERATED_BODY()
public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnClicked,const FGeometry& , FSkillDataSpec &);
	DECLARE_MULTICAST_DELEGATE(FOnDrag);
public:
	FOnClicked m_OnClicked;
	
	FOnDrag m_OnDragDetect;

	FOnDrag m_OnDragEnd;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UImage* m_ImageSkillIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextSkillLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UCheckBox* m_SelectFrame;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UCooldownProgress* m_SkillCooldown;
	
protected:
	UPROPERTY()
	UPlayerDiabloAbilitySystemComp* m_PlDiaComp;
	
	FSkillDataSpec* m_HoldSpec;
	
public:
	void InitSkillButton(FSkillDataSpec& skill_spec,UPlayerDiabloAbilitySystemComp* comp);
	
	void UpdateLevelText();
	
	void SkillSelected();

	void SkillDeselected();

	void CloseHotKeyPanel();
	
	void SetCooldownProgress(float cd, float m_f_max_cd);


private:
	UDiaDragDropSkill* CreateDDO();

	

public:
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	//
	FORCEINLINE FSkillDataSpec* GetSkillSpec()
	{
		return m_HoldSpec;
	}
	friend UDiaDragDropSkill;

	void StartCooldown();

	void ClearCooldown();
};
