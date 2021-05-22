#include "PetDgPanel.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DgContentManagers/DungeonManager.h"
#include "Widgets/HUD/MainCanvas.h"

void UPetDgPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnPetDg01->OnClicked.AddDynamic(this,&UPetDgPanel::EnterPetDg01);

	//m_ImgTxtPetTicket->SetText(FText::FromString(FString::Printf(TEXT("펫 티켓 %d개"),Ticket)));

}

void UPetDgPanel::EnterPetDg01()
{
	if(!UDiabloGameInstance::Get->m_DungeonManager->TryOpenDgKey())
	{
		UDiabloGameInstance::Get->RequestPopupText(TEXT("던전 열쇠 부족"));
		return;
	}
	UDiabloGameInstance::Get->m_PetDgManager->RequestMovePetDg(0);
		
	UDiabloGameInstance::Get->m_DungeonManager->OpenLevel(UDiabloGameInstance::Get->UDiabloGameInstance::Get->m_PetDgManager);
}

