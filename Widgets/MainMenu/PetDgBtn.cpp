// My First Hack n Slash


#include "PetDgBtn.h"

#include "Managers/DiabloGameInstance.h"
#include "Managers/DgContentManagers/DungeonManager.h"

void UPetDgBtn::Init(int index, const FPetDgTableRow* petDg)
{
	m_nIndex=index;
	m_TxtDgName->SetText(FText::FromString(FString::Printf(TEXT("펫 던전:%02d"),index+1)));
	m_BtnEnter->OnClicked.AddDynamic(this,&UPetDgBtn::EnterDungeon);
	m_TxtTicketCost->SetText(FText::FromString(FString::Printf(TEXT("-%d"),petDg->m_nTicketCost)));
	m_TxtRewards->SetText(FText::FromString(FString::Printf(TEXT("성공시 획득 티켓:%d"),petDg->m_nRewardTicket)));
	
	BigInt MobHp =  UDiabloGameInstance::Get->m_PetDgManager->GetPetDgTables()[m_nIndex]->GetMobHp();
	
	m_TxtMonsterHealth->SetText(FText::FromString(FString::Printf(TEXT("HP:%s"),*UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(MobHp,2))));
}


void UPetDgBtn::EnterDungeon()
{
	int Cost = UDiabloGameInstance::Get->m_PetDgManager->GetPetDgTables()[m_nIndex]->m_nTicketCost;
	
	if(!UDiabloGameInstance::Get->m_DungeonManager->TryOpenDgKey(Cost))
	{
		UDiabloGameInstance::Get->RequestPopupText(TEXT("던전 열쇠 부족"));
		return;
	}
	
	UDiabloGameInstance::Get->m_PetDgManager->RequestMovePetDg(m_nIndex);
		
	UDiabloGameInstance::Get->m_DungeonManager->OpenLevel(UDiabloGameInstance::Get->m_PetDgManager);
}
