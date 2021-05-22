// My First Hack n Slash


#include "WeaponDgBtn.h"

#include "Managers/DiabloGameInstance.h"
#include "Managers/DgContentManagers/DungeonManager.h"

void UWeaponDgBtn::Init(int index, const FWeaponDgTableRow* weapon_dg)
{
	m_nIndex=index;
	m_TxtDgName->SetText(FText::FromString(FString::Printf(TEXT("강화석 광산:%02d"),index+1)));
	m_BtnEnter->OnClicked.AddDynamic(this,&UWeaponDgBtn::EnterDungeon);
	m_TxtTicketCost->SetText(FText::FromString(FString::Printf(TEXT("-%d"),weapon_dg->m_nTicketCost)));
	m_TxtRewards->SetText(FText::FromString(FString::Printf(TEXT("처치당 무기 강화석:%d~%d"),weapon_dg->m_nPrizeWeaponStoneMin,weapon_dg->m_nPrizeWeaponStoneMax)));
	
	BigInt MobHp =  UDiabloGameInstance::Get->m_WeaponDgManager->GetWeaponDgTables()[m_nIndex]->GetMobHp();
	
	m_TxtMonsterHealth->SetText(FText::FromString(FString::Printf(TEXT("HP:%s"),*UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(MobHp,2))));
}


void UWeaponDgBtn::EnterDungeon()
{
	int Cost = UDiabloGameInstance::Get->m_WeaponDgManager->GetWeaponDgTables()[m_nIndex]->m_nTicketCost;
	
	if(!UDiabloGameInstance::Get->m_DungeonManager->TryOpenDgKey(Cost))
	{
		UDiabloGameInstance::Get->RequestPopupText(TEXT("던전 열쇠 부족"));
		return;
	}
	
	UDiabloGameInstance::Get->m_WeaponDgManager->RequestMoveWeaponDg(m_nIndex);
		
	UDiabloGameInstance::Get->m_DungeonManager->OpenLevel(UDiabloGameInstance::Get->m_WeaponDgManager);
}
