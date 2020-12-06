#include "DiaSkillPopup.h"

#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"

#define LOCTEXT_NAMESPACE "DiaSkillPopup"
//LOCTEXT("GoodbyeWorld","Goodbye World!")
UDiaSkillPopup::UDiaSkillPopup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_CurrentSkillSpec = nullptr;
	m_PlayerDiaComp = nullptr;
	m_FormatSkillCD = LOCTEXT("SkillPopupCooldown", "Cooldown: {0}");
	m_FormatSkillRank = LOCTEXT("SkillPopupRank", "Rank: {0}");
	m_FormatRequireLevel = LOCTEXT("SkillPopupRequireLevel", "Requires level: {0}");
	m_FormatMainRequire = LOCTEXT("SkillPopupMainRequire", "Main hand Requires: {0}");
	m_FormatSubRequire = LOCTEXT("SkillPopupSubRequire", "Sub hand Requires: {0}");
	m_FormatStanceRequire= LOCTEXT("SkillPopupStanceRequire", "{0} Stance Requires");
	m_TextSkillNotLearn= LOCTEXT("SkillPopupNotLearned", "Not Learned");
	m_FormatDouble = FText::FromString(" Main hand Requires: {0} \n Sub hand Requires: {0} ");
}

void UDiaSkillPopup::Init(UPlayerDiabloAbilitySystemComp* plDiaComp)
{
	m_PlayerDiaComp = plDiaComp;

	m_BtnLearn->OnClicked.AddDynamic(this, &UDiaSkillPopup::LearnSkill);

	m_AnimEndDele.BindDynamic(this,&UDiaSkillPopup::HidePopup);

	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(Slot);
	
	m_InitPos = PanelSlot->GetPosition();
}

void UDiaSkillPopup::SetPopupSkillData(FSkillDataSpec* selectedSkillData,const FGeometry& geo)
{
	if(m_CurrentSkillSpec)
	{
		PlayHideInfoAnim();
		return;
	}
	
	m_CurrentSkillSpec = selectedSkillData;
	check(m_CurrentSkillSpec);

	UnbindAllFromAnimationFinished(m_FadeAnimation);
	
	m_BGForTouch->SetVisibility(ESlateVisibility::Visible);
	
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	PlayAnimation(m_FadeAnimation);
	
	SetSkillIcon();
	SetSkillName();
	SetSkillType();
	SetSkillRank();
	SetResourceText();
	SetCooldownText();
	SetSkillDescription();
	SetSkillUseRequire();
	SetSkillDamageType();
	SetSkillAdditionalInfo();
	SetSkillPreviewOverlay();

	if(m_PlayerDiaComp->GetSkillPoints()<=0)//cant learn
	{
		m_BtnLearn->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		m_BtnLearn->SetVisibility(ESlateVisibility::Visible);
	}

	ForceLayoutPrepass();
	
	SetPanelPosition(geo);
}

void UDiaSkillPopup::SetPanelPosition(const FGeometry& instigatorGeo)
{
	FGeometry&& Geo = UWidgetLayoutLibrary::GetPlayerScreenWidgetGeometry(GetOwningPlayer());

	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(Slot);

	UCanvasPanel* CanvasPanelParent = Cast<UCanvasPanel>(PanelSlot->Parent);

	FVector2D ClickedItemSlot = CanvasPanelParent->GetCachedGeometry().AbsoluteToLocal(instigatorGeo.GetAbsolutePosition()) + instigatorGeo.GetLocalSize() / 2.0f;

	ClickedItemSlot.X += (GetDesiredSize().X / 2.0f) + (instigatorGeo.GetLocalSize().X / 2.0f);

	float ScreenY = Geo.GetAbsoluteSize().Y;

	float PopupSizeY = (GetDesiredSize().Y * Geo.Scale) / 2.0f + 50.f;

	float ScreenTopToItem = ClickedItemSlot.Y * Geo.Scale;

	float ScreenBottomToItem = ScreenY - ClickedItemSlot.Y * Geo.Scale;

	float ReverseScale = 1.f / Geo.Scale;

	if (ScreenTopToItem < PopupSizeY)
	{
		float Diff = PopupSizeY - ScreenTopToItem;

		ClickedItemSlot.Y += Diff * ReverseScale;
		ClickedItemSlot.Y -= 50.f;
	}
	else if (ScreenBottomToItem < PopupSizeY) //아래 공간이 팝업창보다 작을때
	{
		float Diff = PopupSizeY - FMath::Abs(ScreenBottomToItem);

		ClickedItemSlot.Y -= Diff * ReverseScale;
	}

	PanelSlot->SetPosition(ClickedItemSlot);
}

void UDiaSkillPopup::SetSkillIcon()
{
	m_ImageSkillIcon->SetBrushFromTexture(m_CurrentSkillSpec->m_SkillDataPtr->m_SkillIcon);
}

void UDiaSkillPopup::SetSkillName()
{
	m_TextSkillName->SetText(m_CurrentSkillSpec->m_SkillDataPtr->m_SkillShowingName);
}

void UDiaSkillPopup::SetSkillType()
{
	m_TextSkillType->SetText(m_CurrentSkillSpec->m_SkillDataPtr->m_TagSkillCastType);
}

void UDiaSkillPopup::SetSkillRank()
{
	if(m_CurrentSkillSpec->m_nCurrentLevel<=0)
	{
		m_TextSkillRank->SetText(m_TextSkillNotLearn);
	}
	else
	{
		FFormatOrderedArguments Args;

		Args.Add(m_CurrentSkillSpec->m_nCurrentLevel);

		m_TextSkillRank->SetText(FText::Format(m_FormatSkillRank, Args));
	}
}

void UDiaSkillPopup::SetResourceText()
{
	m_TextResource->SetText(m_CurrentSkillSpec->GetCostText());
}

void UDiaSkillPopup::SetCooldownText()
{
	float CD = m_CurrentSkillSpec->m_SkillDataPtr->m_fSkillCD.m_fScaleFloat.GetValueAtLevel(m_CurrentSkillSpec->m_nCurrentLevel);

	float CDRate = 1.f - UPlayerDiabloAttribute::Get->GetCoolDownReduce();

	CD *= CDRate;

	if (CD <= 0.f)
	{
		m_TextCooldown->SetVisibility(ESlateVisibility::Collapsed);

		return;
	}

	FFormatOrderedArguments Args;

	Args.Add(CD);

	m_TextCooldown->SetVisibility(ESlateVisibility::HitTestInvisible);

	m_TextCooldown->SetText(FText::Format(m_FormatSkillCD, Args));
}

void UDiaSkillPopup::SetSkillDescription()
{
	m_TextSkillDesc->SetText(m_CurrentSkillSpec->GetDescFormatText());

	m_TextSkillDesc->ForceLayoutPrepass();
}

void UDiaSkillPopup::SetSkillUseRequire()
{
	if(!m_CurrentSkillSpec->m_SkillDataPtr->m_RequireAnimStance.IsNull())
	{
		FFormatOrderedArguments AArgs;

		AArgs.Add(m_CurrentSkillSpec->m_SkillDataPtr->m_RequireAnimStance.GetRow<FAnimStance>("")->m_ShowingText);

		m_TextUseSkillRequireLimit->SetText(FText::Format(m_FormatStanceRequire, AArgs));

		return;
	}


	FText RR = m_CurrentSkillSpec->m_SkillDataPtr->m_RequireMainWeaponType.IsNull() ? FText(): m_CurrentSkillSpec->m_SkillDataPtr->m_RequireMainWeaponType.GetRow<FItemType>("")->m_ShowingName;

	FText LL = m_CurrentSkillSpec->m_SkillDataPtr->m_RequireSubWeaponType.IsNull() ? FText(): m_CurrentSkillSpec->m_SkillDataPtr->m_RequireSubWeaponType.GetRow<FItemType>("")->m_ShowingName;

	m_TextUseSkillRequireLimit->SetVisibility(ESlateVisibility::Visible);

	if (!RR.IsEmpty())
	{
		FFormatOrderedArguments RArgs;

		RArgs.Add(RR);

		FText RightOnlyText = FText::Format(m_FormatMainRequire, RArgs);

		if (!LL.IsEmpty()) //left both
		{
			FFormatOrderedArguments BothArgs;

			BothArgs.Add(RR);

			BothArgs.Add(LL);

			FText DoubleBothText = FText::Format(m_FormatDouble, BothArgs);

			m_TextUseSkillRequireLimit->SetText(DoubleBothText);
		}
		else
		{
			m_TextUseSkillRequireLimit->SetText(RightOnlyText);
		}
	}
	else if (!LL.IsEmpty())
	{
		FFormatOrderedArguments LArgs;

		LArgs.Add(LL);

		FText LeftOnlyText = FText::Format(m_FormatSubRequire, LArgs);

		m_TextUseSkillRequireLimit->SetText(LeftOnlyText);
	}
	else
	{
		m_TextUseSkillRequireLimit->SetVisibility(ESlateVisibility::Collapsed);
	}

	m_TextUseSkillRequireLimit->ForceLayoutPrepass();
}

void UDiaSkillPopup::SetSkillDamageType()
{
	m_ImageTextSkillDamageType->SetString(
		m_AryDmgTypeTexts[static_cast<int>(m_CurrentSkillSpec->m_SkillDataPtr->m_eSkillDamageType)]);
	m_ImageTextSkillDamageType->SetIcon(
		m_AryDmgTypeIcons[static_cast<int>(m_CurrentSkillSpec->m_SkillDataPtr->m_eSkillDamageType)]);
}

void UDiaSkillPopup::SetSkillAdditionalInfo()
{
	if (m_CurrentSkillSpec->m_SkillDataPtr->m_TextSkillAdditionalEffect.IsEmpty())
	{
		m_TextAdditionalInfo->SetVisibility(ESlateVisibility::Collapsed);

		return;
	}

	m_TextAdditionalInfo->SetVisibility(ESlateVisibility::HitTestInvisible);
	m_TextAdditionalInfo->SetText(m_CurrentSkillSpec->m_SkillDataPtr->m_TextSkillAdditionalEffect);
	m_TextAdditionalInfo->ForceLayoutPrepass();
}

void UDiaSkillPopup::SetSkillPreviewOverlay()
{
	if (m_CurrentSkillSpec->m_nCurrentLevel >= m_CurrentSkillSpec->m_SkillDataPtr->m_nMaxSkillLevel)
	{
		m_OverlayNextPreview->SetVisibility(ESlateVisibility::Collapsed);

		return;
	}

	m_OverlayNextPreview->SetVisibility(ESlateVisibility::HitTestInvisible);

	m_TextPreviewSkillEffect->SetText(m_CurrentSkillSpec->GetLevelupPreviewFormatText());
	m_TextPreviewSkillEffect->ForceLayoutPrepass();

	int RequireLevel = m_CurrentSkillSpec->GetRequireLearnLevel();
	
	if(RequireLevel <= ADiabloPlayerController::Get->GetPlayerPawn()->GetCharacterLevel())
	{
		m_TextPreviewSkillRequireLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		FFormatOrderedArguments Args;

		Args.Add(m_CurrentSkillSpec->GetRequireLearnLevel());

		m_TextPreviewSkillRequireLevel->SetText(FText::Format(m_FormatRequireLevel, Args));
	}
	else
	{
		m_TextPreviewSkillRequireLevel->SetVisibility(ESlateVisibility::Collapsed);
	}

	m_OverlayNextPreview->ForceLayoutPrepass();
}

FReply UDiaSkillPopup::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Rep = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	PlayHideInfoAnim();
	//m_OnActionEnd.Broadcast();
	return Rep;
}

void UDiaSkillPopup::LearnSkill()
{
	if(m_CurrentSkillSpec)
		m_PlayerDiaComp->LevelupSkill(m_CurrentSkillSpec);
}

void UDiaSkillPopup::PlayHideInfoAnim(float delay)
{
	m_CurrentSkillSpec=nullptr;//prevent double touch
	
	BindToAnimationFinished(m_FadeAnimation, m_AnimEndDele);

	m_BGForTouch->SetVisibility(ESlateVisibility::HitTestInvisible);
    
	PlayAnimationReverse(m_FadeAnimation);
}

void UDiaSkillPopup::HidePopup()
{
	PRINTF("HideInfoPanel");
    
	m_CurrentSkillSpec=nullptr;
    
	SetVisibility(ESlateVisibility::Collapsed);
    
	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(Slot);
    
	PanelSlot->SetPosition(m_InitPos);
}

#undef LOCTEXT_NAMESPACE
