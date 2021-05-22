#include "PetDgPanel.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DgContentManagers/DungeonManager.h"
#include "Widgets/HUD/MainCanvas.h"

void UPetDgPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	
	int Index = 0;

	for(const auto* PetDg :  UDiabloGameInstance::Get->m_PetDgManager->GetPetDgTables())
	{
		UPetDgBtn* CreatedBtn = CreateWidget<UPetDgBtn>(this, m_ClassPetDgBtn);

		CreatedBtn->Init(Index,PetDg);

		m_VertiList->AddChild(CreatedBtn);

		CreatedBtn->SetPadding(FMargin(10, 10, 10, 10));

		Index++;
	}

}

void UPetDgPanel::EnterPetDg01()
{
	int Cost = UDiabloGameInstance::Get->m_PetDgManager->GetCurrentDg()->m_nTicketCost;
	
	if(!UDiabloGameInstance::Get->m_DungeonManager->TryOpenDgKey(Cost))
	{
		UDiabloGameInstance::Get->RequestPopupText(TEXT("던전 열쇠 부족"));
		return;
	}
	UDiabloGameInstance::Get->m_PetDgManager->RequestMovePetDg(0);
		
	UDiabloGameInstance::Get->m_DungeonManager->OpenLevel(UDiabloGameInstance::Get->UDiabloGameInstance::Get->m_PetDgManager);
}

