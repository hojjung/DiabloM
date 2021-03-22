#include "QuestButton.h"

#include "Managers/DiabloGameInstance.h"


void UQuestButton::Init(int index,FQuestDataSpec* dataSpecPTr)
{
	m_nIndex = index;

	m_CurrentSpec = dataSpecPTr;

	m_BtnComplete->OnClicked.AddDynamic(this,&UQuestButton::OnClickButton);

	UpdateQuestWidget();
}

void UQuestButton::UpdateQuestWidget()
{
	m_TextQuestName->SetText(m_CurrentSpec->m_Data->m_TextQuestName);

	m_TextQuestGauge->SetText(m_CurrentSpec->GetGaugeFormatTxt());
	
	m_ProgressBar->SetRenderOpacity(m_CurrentSpec->GetGaugePercent());

	m_TextImgComplete->SetText(FText::AsNumber(m_CurrentSpec->GetCompletePrize()));

	m_BtnComplete->SetIsEnabled(m_CurrentSpec->IsCompletable());
}

void UQuestButton::OnClickButton()
{
	if(TryComplete(m_nIndex))
	{
		UpdateQuestWidget();
	}
}

bool UQuestButton::TryComplete(int index)
{
	return UDiabloGameInstance::Get->m_QuestManager->CompleteQuest(index);
}
