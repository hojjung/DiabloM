#include "MagicDgPanel.h"

#include "Managers/DiabloGameInstance.h"
#include "Managers/DgContentManagers/DungeonManager.h"

void UMagicDgPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnMagicDg01->OnClicked.AddDynamic(this,&UMagicDgPanel::EnterMagicDg01);
}

void UMagicDgPanel::EnterMagicDg01()
{
	UDiabloGameInstance::Get->m_MagicDgManager->RequestMoveMagicStoneDg(0);
		
	UDiabloGameInstance::Get->m_DungeonManager->OpenLevel(UDiabloGameInstance::Get->m_MagicDgManager);
}
