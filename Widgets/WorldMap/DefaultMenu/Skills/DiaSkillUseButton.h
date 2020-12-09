// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "TextBlock.h"
#include "AbilitySystem/Components/PlayerDiabloAbilitySystemComp.h"
#include "Blueprint/UserWidget.h"
#include "Datas/SkillDataTable.h"
#include "Widgets/CommonElement/CooldownProgress.h"

#include "DiaSkillUseButton.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UDiaSkillUseButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDiaSkillUseButton(const FObjectInitializer& objInit);

	void Init(UPlayerDiabloAbilitySystemComp* diaComp,int index);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UButton* m_BtnSkill;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UCooldownProgress* m_SkillCooldown;
	
protected:
	int m_nIndex;

	float m_fMaxCD; 
	
	FSkillDataSpec* m_EquippedSkillSpec;

	FGameplayAbilitySpec* m_GaSpec;
	
	TWeakObjectPtr<UPlayerDiabloAbilitySystemComp> m_PlayerDiaComp;
	
public:
	void SetSkillSpec(FSkillDataSpec* skillSpec);

	void ClearSkillSpec();

	void ClearCooldown();

	UFUNCTION()
	void UseSkill();
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
