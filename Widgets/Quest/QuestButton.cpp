#include "QuestButton.h"

#include "Managers/DiabloGameInstance.h"


void UQuestButton::Init(int index,FQuestDataSpec* dataSpecPTr)
{
	m_nIndex = index;

	m_CurrentSpec = dataSpecPTr;

	m_BtnComplete->OnClicked.AddDynamic(this,&UQuestButton::OnClickButton);
	m_BtnComplete->OnClicked.AddDynamic(this,&UQuestButton::ChargeStart);
	m_BtnComplete->OnHovered.AddDynamic(this,&UQuestButton::ChargeStart);
	m_BtnComplete->OnUnhovered.AddDynamic(this,&UQuestButton::ChargeEnd);
	m_BtnComplete->OnReleased.AddDynamic(this,&UQuestButton::ChargeEnd);

	UpdateQuestWidget();
}

void UQuestButton::UpdateQuestWidget()
{
	if(m_CurrentSpec->IsMaxLv())
	{
		m_TextImgComplete->SetText(FText::FromString(TEXT("MAX")));
		
		m_BtnComplete->SetIsEnabled(false);
		return;
	}
	
	m_TextQuestName->SetText(m_CurrentSpec->m_Data->m_TextQuestName);

	m_TextQuestGauge->SetText(m_CurrentSpec->GetGaugeFormatTxt());

	m_ProgressBar->SetProgressValue(m_CurrentSpec->GetGaugePercent());

	
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

void UQuestButton::ChargeStart()
{m_bChargeUpgrade = true;
	m_fDeltaCounter= 0.f;
}

void UQuestButton::ChargeEnd()
{m_bChargeUpgrade = false;
	m_fDeltaCounter= 0.f;
}

void UQuestButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(!m_bChargeUpgrade)
	{
		return;
	}

	m_fDeltaCounter+=InDeltaTime;

	if(m_fDeltaCounter>0.1f)
	{
		m_fDeltaCounter= 0.f;
		OnClickButton();
		//upgradeTick
	}
}
