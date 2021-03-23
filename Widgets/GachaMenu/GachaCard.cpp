#include "GachaCard.h"
#include "Datas/GachaDataTable.h"

void UGachaCard::SetVisual(const FGachaAbleRow* tableData)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_ImgIcon->SetBrushFromTexture(tableData->m_Icon);

	FItemTierTableRow* Table = tableData->m_Handle.GetRow<FItemTierTableRow>("");
	
	m_ImgGlow->SetBrushTintColor(Table->m_TierColor);

	m_TextTierName->SetText(Table->m_TextTierName);
	
	m_TextShowingName->SetText(tableData->m_ShowingName);

	if(Table->m_bHasSpecialEffect)
	{
		ShowSpecialEffect();
		
		m_TextTierName->SetVisibility(ESlateVisibility::Collapsed);
		m_TextShowingName->SetVisibility(ESlateVisibility::Collapsed);
		m_ImgIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGachaCard::ShowSpecialEffect()
{
	PlayAnimation(m_AnimFlash);
}

void UGachaCard::OnAnimationFinishedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationFinishedPlaying(Player);

	m_TextTierName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	m_TextShowingName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	m_ImgIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
