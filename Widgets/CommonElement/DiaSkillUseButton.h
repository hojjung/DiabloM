
#pragma once

#include "CoreMinimal.h"
#include "TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Datas/PlayerUpgradeData.h"
#include "Widgets/CommonElement/CooldownProgress.h"
#include "Widgets/CommonElement/Joystick.h"
#include "Widgets/UpgradeMenu/SkillUpgradeButton.h"



#include "DiaSkillUseButton.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UDiaSkillUseButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPressChange,UWidget*);

	FOnPressChange m_OnPressed;

	FOnPressChange m_OnReleased;

	FDelegateHandle m_DeleHandle;
	
	void Init(int index);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UJoystick* m_Joystick;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UCooldownProgress* m_SkillCooldown;
	
public:
	int m_nIndex;

	float m_fMaxCD;

	float m_fCurrentCD;

	bool m_bIsPressing;

	bool m_bIsSkillUsable;

	bool m_bIsDragSkill;
	
	FSkillSpec* m_EquippedSkillSpec;

	
public:
	void SetSkillSpec(FSkillSpec* skillSpec);

	void UseSkill();
	
	void ClearSkillSpec();

	void ClearCooldown();

	UFUNCTION()
	void OnPressBtn();

	UFUNCTION()
    void OnReleaseBtn();
	
	void TryStartCooldown();

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	void CustomTickForBind(float delta);
};
