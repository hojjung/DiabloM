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
	if(!UDiabloGameInstance::Get->m_DungeonManager->TryOpenDgKey())
	{
		UDiabloGameInstance::Get->RequestPopupText(TEXT("던전 열쇠 부족"));
		return;
	}
	UDiabloGameInstance::Get->m_MagicDgManager->RequestMoveMagicStoneDg(0);
		
	UDiabloGameInstance::Get->m_DungeonManager->OpenLevel(UDiabloGameInstance::Get->m_MagicDgManager);
}
