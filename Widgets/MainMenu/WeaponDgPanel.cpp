// My First Hack n Slash


#include "WeaponDgPanel.h"

#include "Managers/DiabloGameInstance.h"
#include "Managers/DgContentManagers/DungeonManager.h"

//WidgetBlueprint'/Game/Blueprints/Widget/MainMenu/WB_WeaponDgBtn.WB_WeaponDgBtn'

void UWeaponDgPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	int Index = 0;

	for(const auto* WeaponDg :  UDiabloGameInstance::Get->m_WeaponDgManager->GetWeaponDgTables())
	{
		UWeaponDgBtn* CreatedBtn = CreateWidget<UWeaponDgBtn>(this, m_ClassWeaponDgBtn);

		CreatedBtn->Init(Index,WeaponDg);

		m_VertiList->AddChild(CreatedBtn);

		CreatedBtn->SetPadding(FMargin(10, 10, 10, 10));

		Index++;
	}

}

void UWeaponDgPanel::EnterWeaponDg01()
{
	int Cost = UDiabloGameInstance::Get->m_WeaponDgManager->GetCurrentDg()->m_nTicketCost;
	
	if(!UDiabloGameInstance::Get->m_DungeonManager->TryOpenDgKey(Cost))
	{
		UDiabloGameInstance::Get->RequestPopupText(TEXT("던전 열쇠 부족"));
		return;
	}
	UDiabloGameInstance::Get->m_WeaponDgManager->RequestMoveWeaponDg(0);
	UDiabloGameInstance::Get->m_DungeonManager->OpenLevel(UDiabloGameInstance::Get->m_WeaponDgManager);
}
