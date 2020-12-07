#pragma once

#include "DiabloM.h"

#include "Image.h"
#include "TextBlock.h"
#include "AbilitySystem/Components/PlayerDiabloAbilitySystemComp.h"
#include "Blueprint/UserWidget.h"
#include "Datas/SkillDataTable.h"
#include "Widgets/CommonElement/ImageAndText.h"

#include "DiaSkillPopup.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UDiaSkillPopup : public UUserWidget
{
	GENERATED_BODY()
public:
	UDiaSkillPopup(const FObjectInitializer& ObjectInitializer); 

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UImage* m_ImageSkillIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextSkillName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextSkillType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextSkillRank;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextResource;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextCooldown;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextSkillDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextUseSkillRequireLimit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UImageAndText* m_ImageTextSkillDamageType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextAdditionalInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UOverlay* m_OverlayNextPreview;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextPreviewSkillEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextPreviewSkillRequireLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UButton* m_BtnLearn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImage* m_BGForTouch;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UTexture2D*> m_AryDmgTypeIcons;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FText> m_AryDmgTypeTexts;
protected:
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* m_FadeAnimation;
	UPROPERTY()
	UPlayerDiabloAbilitySystemComp* m_PlayerDiaComp;
	
	FSkillDataSpec* m_CurrentSkillSpec;

	FTextFormat m_FormatDouble;

	FTextFormat m_FormatStanceRequire;

	FTextFormat m_FormatMainRequire;

	FTextFormat m_FormatSubRequire;

	FTextFormat m_FormatSkillCD;
	
	FTextFormat m_FormatSkillRank;

	FTextFormat m_FormatRequireLevel;

	FText m_TextSkillNotLearn;

	FWidgetAnimationDynamicEvent m_AnimEndDele;
	
	FVector2D m_InitPos;

	//무기 타입별 필요한것은 무기 타입한테서 데이터테이블 작성으로 가저오자
	
public:
	void Init(UPlayerDiabloAbilitySystemComp* plDiaComp);
	
	void SetSkillSpecData(FSkillDataSpec* selectedSkillData);

	void SetSkillPopupWidget(FSkillDataSpec* selectedSkillData,const FGeometry& geo);
	
	void SetPanelPosition(const FGeometry& instigatorGeo);//RightOnly

private:
	void SetSkillIcon();

	void SetSkillName();

	void SetSkillType();

	void SetSkillRank();

	void SetResourceText();

	void SetCooldownText();

	void SetSkillDescription();

	void SetSkillUseRequire();

	void SetSkillDamageType();

	void SetSkillAdditionalInfo();

	void SetSkillPreviewOverlay();

	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )override;

public:
	UFUNCTION()
	void LearnSkill();
	UFUNCTION(BlueprintCallable,Category="Info")
    void PlayHideInfoAnim(float delay=0.f);
	UFUNCTION()
	void HidePopup();
};
