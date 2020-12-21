// My First Hack n Slash


#include "MapSelectButton.h"

UMapSelectButton::UMapSelectButton(const FObjectInitializer& objInit):Super(objInit)
{
	m_DgData=nullptr;
}

void UMapSelectButton::InitButton(const FDungeonDataRow* dgData)
{
	if(!dgData)
	{
		return;
	}
	m_DgData=dgData;
	m_ImgText->SetIcon(m_DgData->m_DgIcon);
	m_ImgText->SetString(m_DgData->m_DgShowName);
}

void UMapSelectButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnOpenPopup->OnClicked.AddDynamic(this,&UMapSelectButton::OnBttnClick);
}

void UMapSelectButton::OnBttnClick()
{
	if(m_DgData)
	{
		m_OnDgClicked.Broadcast(m_DgData);
	}
}
