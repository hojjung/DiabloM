#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "Datas/DiaTechnologyAsset.h"

#include "DiaTalentPopup.generated.h"

class UPlayerDiabloAbilitySystemComp;
UCLASS()
class DIABLOM_API UDiaTalentPopup : public UUserWidget
{
	GENERATED_BODY()

public:
	UDiaTalentPopup(const FObjectInitializer& ObjectInitializer); 

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UImage* m_ImageTalentIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextTalentName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextTalentRank;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextTalentDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UOverlay* m_OverlayRequireLimit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextRequirePointLimit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextAdditionalInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UOverlay* m_OverlayNextPreview;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UTextBlock* m_TextPreviewTalentEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Skill")
	UButton* m_BtnLearn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Info")
	UImage* m_BGForTouch;
protected:
	UPROPERTY(meta = (BindWidgetAnim))
	UWidgetAnimation* m_FadeAnimation;
	UPROPERTY()
	UPlayerDiabloAbilitySystemComp* m_PlayerDiaComp;
	UPROPERTY()
	UDiaTechnologyAsset* m_CurrentTalent;
	
	FWidgetAnimationDynamicEvent m_AnimEndDele;
	
	FVector2D m_InitPos;

	//무기 타입별 필요한것은 무기 타입한테서 데이터테이블 작성으로 가저오자
public:
	void Init(UPlayerDiabloAbilitySystemComp* plDiaComp);
	
	void SetTalentPopupWidget(UDiaTechnologyAsset* selectedSkillData,const FGeometry& geo);
	
private:
	void SetTalentIcon();
	void SetTalentName();
	void SetTalentRank();
	void SetTalentDesc();
	void SetRequirePointAlertShow();
	void SetAdditionalInfoShow();
	void SetTalentLevelupPreview();

	
	void SetPanelPosition(const FGeometry& instigatorGeo);//RightOnly
	
	void SetTalentSpecData(UDiaTechnologyAsset* selectedTalentData);
	
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )override;

public:
	UFUNCTION()
	void LearnTalent();
	UFUNCTION(BlueprintCallable,Category="Info")
    void PlayHideInfoAnim(float delay=0.f);
	UFUNCTION()
	void HidePopup();
};
