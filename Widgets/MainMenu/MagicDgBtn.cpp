// My First Hack n Slash


#include "MagicDgBtn.h"

#include "Managers/DiabloGameInstance.h"
#include "Managers/DgContentManagers/DungeonManager.h"

void UMagicDgBtn::Init(int index, const FMagicDgTableRow* magic_dg)
{
	m_nIndex=index;
	m_TxtDgName->SetText(FText::FromString(FString::Printf(TEXT("마정석 드래곤 둥지:%02d"),index+1)));
	m_BtnEnter->OnClicked.AddDynamic(this,&UMagicDgBtn::EnterDungeon);
	m_TxtTicketCost->SetText(FText::FromString(FString::Printf(TEXT("-%d"),magic_dg->m_nTicketCost)));
	m_TxtRewards->SetText(FText::FromString(FString::Printf(TEXT("획득 가능 마정석:%d~%d"),magic_dg->m_nPrizeMagicStoneMin,magic_dg->m_nPrizeMagicStoneMax)));
	
	BigInt MobHp =  UDiabloGameInstance::Get->m_MagicDgManager->GetMagicDgData()[m_nIndex]->GetMobHp();
	
	m_TxtMonsterHealth->SetText(FText::FromString(FString::Printf(TEXT("HP:%s"),*UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(MobHp,2))));
}

void UMagicDgBtn::EnterDungeon()
{
	int Cost = UDiabloGameInstance::Get->m_MagicDgManager->GetMagicDgData()[m_nIndex]->m_nTicketCost;
	
	if(!UDiabloGameInstance::Get->m_DungeonManager->TryOpenDgKey(Cost))
	{
		UDiabloGameInstance::Get->RequestPopupText(TEXT("던전 열쇠 부족"));
		return;
	}
	
	UDiabloGameInstance::Get->m_MagicDgManager->RequestMoveMagicStoneDg(m_nIndex);
		
	UDiabloGameInstance::Get->m_DungeonManager->OpenLevel(UDiabloGameInstance::Get->m_MagicDgManager);
}
