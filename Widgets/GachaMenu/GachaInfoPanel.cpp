#include "GachaInfoPanel.h"


void UGachaInfoPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_AryGachaInfoElement.Reset();
	
	int i=0;

	while (i < 30)
	{
		UGachaInfoElement* CreatedBtn = CreateWidget<UGachaInfoElement>(this, m_ClassInfoElement);

		m_Verti->AddChild(CreatedBtn);

		CreatedBtn->SetPadding(FMargin(10, 10, 10, 10));

		m_AryGachaInfoElement.Add(CreatedBtn);

		CreatedBtn->SetVisibility(ESlateVisibility::Collapsed);

		i++;
	}
	
	m_BtnClose->OnClicked.AddDynamic(this,&UGachaInfoPanel::CloseGachaPanel);
}

void UGachaInfoPanel::OpenGachaInfo(TArray<FGachaTableRow*>& aryTableRow,float totalPercent)
{
	
	for(UGachaInfoElement* GachaElement : m_AryGachaInfoElement)
	{
		GachaElement->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	int index=0;
	
	for(const FGachaTableRow* GachaTable : aryTableRow)
	{
		m_AryGachaInfoElement[index]->SetInfoText(GachaTable,totalPercent);
		
		m_AryGachaInfoElement[index]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		index++;
	}

	m_Verti->ScrollToStart();
}

void UGachaInfoPanel::CloseGachaPanel()
{
	SetVisibility(ESlateVisibility::Collapsed);

	for(UGachaInfoElement* GachaElement : m_AryGachaInfoElement)
	{
		GachaElement->SetVisibility(ESlateVisibility::Collapsed);
	}
}
