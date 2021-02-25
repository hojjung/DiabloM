
#pragma once

#include "CoreMinimal.h"
#include "TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CommonElement/CooldownProgress.h"
#include "Widgets/CommonElement/Joystick.h"


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
	
	void Init(int index);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UJoystick* m_Joystick;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UCooldownProgress* m_SkillCooldown;
	
protected:
	int m_nIndex;

	float m_fMaxCD;

	float m_fCurrentCD;

	bool m_bIsPressing;

	bool m_bIsSkillUsable;

	bool m_bIsDragSkill;
	
	//FSkillDataSpec* m_EquippedSkillSpec;

	
public:
	//void SetSkillSpec(FSkillDataSpec* skillSpec);

	void UseSkill();
	
	void ClearSkillSpec();

	void ClearCooldown();

	UFUNCTION()
	void OnPressBtn();

	UFUNCTION()
    void OnReleaseBtn();
	
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

};
