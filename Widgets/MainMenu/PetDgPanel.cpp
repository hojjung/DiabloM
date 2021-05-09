// My First Hack n Slash


#include "PetDgPanel.h"

#include "Managers/DiabloGameInstance.h"
#include "Managers/DgContentManagers/DungeonManager.h"

void UPetDgPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnPetDg01->OnClicked.AddDynamic(this,&UPetDgPanel::EnterPetDg01);
}

void UPetDgPanel::EnterPetDg01()
{
	UDiabloGameInstance::Get->m_PetDgManager->RequestMovePetDg(0);
		
	UDiabloGameInstance::Get->m_DungeonManager->OpenLevel(UDiabloGameInstance::Get->UDiabloGameInstance::Get->m_PetDgManager);
}
