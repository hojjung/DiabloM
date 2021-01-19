
#pragma once

#include "CoreMinimal.h"
#include "Button.h"
#include "Blueprint/UserWidget.h"
#include "Characters/PlayerDiabloCharacter.h"

#include "PotionUseBtn.generated.h"

class UPlayerDiabloAbilitySystemComp;
class UCooldownProgress;
/**
 * 
 */
UCLASS()
class DIABLOM_API UPotionUseBtn : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UButton* m_UseBtn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UCooldownProgress* m_SkillCooldown;
	UPROPERTY()
	APlayerDiabloCharacter* m_Player;
	UPROPERTY()
	UPlayerDiabloAbilitySystemComp* m_PlayerDiaComp;
	
	FGameplayAbilitySpec* m_GaSpec;
	
	float m_fMaxCD;

public:
	void Init(APlayerDiabloCharacter* playerPawn,UPlayerDiabloAbilitySystemComp* diaComp);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void ClearCooldown();

public:
	UFUNCTION()
	void UsePotion();
};
