#include "GachaCardGridPanel.h"


#include "UniformGridSlot.h"
#include "Managers/DiabloGameInstance.h"


void UGachaCardGridPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnClose->OnClicked.AddDynamic(this, &UGachaCardGridPanel::ClosePanel);
	m_BtnConfirm->OnClicked.AddDynamic(this, &UGachaCardGridPanel::ClosePanel);
	m_BtnGachaOneTime->OnClicked.AddDynamic(this, &UGachaCardGridPanel::RollGachaOneTime);
	m_BtnGachaElevenTime->OnClicked.AddDynamic(this, &UGachaCardGridPanel::RollGachaElevenTime);
	m_BtnGachaFiftyTime->OnClicked.AddDynamic(this, &UGachaCardGridPanel::RollGachaFiftyTime);
	//
	m_AryGachaCard.Reset();

	check(m_ClassGachaCard);
	
	for(int y=0; y< 8; y++)
	{
		for(int x=0; x< 7; x++)
		{
			UGachaCard* CreatedCard = CreateWidget<UGachaCard>(this, m_ClassGachaCard);

			auto* SlotUni = m_Grid->AddChildToUniformGrid(CreatedCard,y,x);//20//15

			SlotUni->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
			SlotUni->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);

			m_AryGachaCard.Add(CreatedCard);
			CreatedCard->SetVisibility(ESlateVisibility::Collapsed);
		}		
	}
}

void UGachaCardGridPanel::SetRollGachaData(ERollItemType gachaTable)
{
	m_CurrentType = gachaTable;
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UGachaCardGridPanel::HideCardGrid()
{
	for(UGachaCard* Card : m_AryGachaCard)
	{
		Card->SetVisibility(ESlateVisibility::Collapsed);
	}
}


void UGachaCardGridPanel::ClosePanel()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UGachaCardGridPanel::RollGachaOneTime()
{
	HideCardGrid();
	m_AryGachaCard[0]->SetVisual(UDiabloGameInstance::Get->m_GachaManager->RollItem(m_CurrentType));//card set need
}

void UGachaCardGridPanel::RollGachaElevenTime()
{
	HideCardGrid();
	
	int i = 0;
	
	int j=0;

	while (i < 11)
	{
		if(j==4 || j==11 || j==18 || j==25)
		{
			j+=3;
		}
		
		m_AryGachaCard[j]->SetVisual(UDiabloGameInstance::Get->m_GachaManager->RollItem(m_CurrentType));
		
		j++;

		i++;
	}
}

void UGachaCardGridPanel::RollGachaFiftyTime()
{
	HideCardGrid();
	
	int i = 0;

	while (i < 55)
	{
		m_AryGachaCard[i]->SetVisual(UDiabloGameInstance::Get->m_GachaManager->RollItem(m_CurrentType));

		i++;
	}
}


