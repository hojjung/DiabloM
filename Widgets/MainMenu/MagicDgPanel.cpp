#include "MagicDgPanel.h"

#include "Managers/DiabloGameInstance.h"
#include "Managers/DgContentManagers/DungeonManager.h"

void UMagicDgPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	int Index = 0;

	for(const auto* MagicDg :  UDiabloGameInstance::Get->m_MagicDgManager->GetMagicDgData())
	{
		UMagicDgBtn* CreatedBtn = CreateWidget<UMagicDgBtn>(this, m_ClassMagicDgBtn);

		CreatedBtn->Init(Index,MagicDg);

		m_VertiList->AddChild(CreatedBtn);

		CreatedBtn->SetPadding(FMargin(10, 10, 10, 10));

		Index++;
	}

}

void UMagicDgPanel::EnterMagicDg01()
{
	
}
