// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"


#include "CheckBox.h"
#include "DiaDragDropSkill.h"
#include "Blueprint/UserWidget.h"
#include "Datas/SkillDataTable.h"

#include "SkillLearnButton.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API USkillLearnButton : public UUserWidget
{
	GENERATED_BODY()
public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnClicked,const FGeometry& , FSkillDataSpec &);
	DECLARE_DELEGATE(FOnDrag);
public:
	FOnClicked m_OnClicked;
	
	FOnDrag m_OnDragDetect;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UImage* m_ImageSkillIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextSkillLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UCheckBox* m_SelectFrame;
	
protected:
	FSkillDataSpec* m_HoldSpec;
	
public:
	void InitSkillButton(FSkillDataSpec& skill_spec);

	void SkillSelected();

	void SkillDeselected();
	
private:
	UDiaDragDropSkill* CreateDDO();

public:
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	//
	friend UDiaDragDropSkill;
};
