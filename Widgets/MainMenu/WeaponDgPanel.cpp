// My First Hack n Slash


#include "WeaponDgPanel.h"

#include "Managers/DiabloGameInstance.h"
#include "Managers/DgContentManagers/DungeonManager.h"

void UWeaponDgPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnWeaponDg01->OnClicked.AddDynamic(this,&UWeaponDgPanel::EnterWeaponDg01);
}

void UWeaponDgPanel::EnterWeaponDg01()
{
	if(!UDiabloGameInstance::Get->m_DungeonManager->TryOpenDgKey())
	{
		UDiabloGameInstance::Get->RequestPopupText(TEXT("던전 열쇠 부족"));
		return;
	}
	UDiabloGameInstance::Get->m_WeaponDgManager->RequestMoveWeaponDg(0);
	UDiabloGameInstance::Get->m_DungeonManager->OpenLevel(UDiabloGameInstance::Get->m_WeaponDgManager);
}
