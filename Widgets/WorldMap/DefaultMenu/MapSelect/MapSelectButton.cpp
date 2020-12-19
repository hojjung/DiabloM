// My First Hack n Slash


#include "MapSelectButton.h"

UMapSelectButton::UMapSelectButton(const FObjectInitializer& objInit):Super(objInit)
{
	m_DgData=nullptr;
}

void UMapSelectButton::InitButton(const FDungeonDataRow* dgData)
{
	m_DgData=dgData;
	m_ImgText->SetIcon(m_DgData->m_DgIcon);
	m_ImgText->SetString(m_DgData->m_DgShowName);
}
