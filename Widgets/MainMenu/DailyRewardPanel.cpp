// My First Hack n Slash


#include "DailyRewardPanel.h"


#include "UniformGridSlot.h"
#include "Managers/DiabloGameInstance.h"

void UDailyRewardPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	int OfflineHours = UDiabloGameInstance::Get->m_PlayfabManager->m_nOfflineHours;

	

	
	m_AryClaimGemstone.Reserve(25);
	m_AryClaimGemstone.Add(300);
	m_AryClaimGemstone.Add(1500);
	m_AryClaimGemstone.Add(300);
	m_AryClaimGemstone.Add(300);
	m_AryClaimGemstone.Add(2100);
	m_AryClaimGemstone.Add(300);
	m_AryClaimGemstone.Add(3000);
	m_AryClaimGemstone.Add(450);
	m_AryClaimGemstone.Add(450);
	m_AryClaimGemstone.Add(3600);
	m_AryClaimGemstone.Add(450);
	m_AryClaimGemstone.Add(450);
	m_AryClaimGemstone.Add(450);
	m_AryClaimGemstone.Add(450);
	m_AryClaimGemstone.Add(4500);
	m_AryClaimGemstone.Add(450);
	m_AryClaimGemstone.Add(450);
	m_AryClaimGemstone.Add(450);
	m_AryClaimGemstone.Add(450);
	m_AryClaimGemstone.Add(5100);
	m_AryClaimGemstone.Add(600);
	m_AryClaimGemstone.Add(600);
	m_AryClaimGemstone.Add(600);
	m_AryClaimGemstone.Add(600);
	m_AryClaimGemstone.Add(6000);

	int MaxDay = m_AryClaimGemstone.Num();

	m_nDDay = UDiabloGameInstance::Get->m_ShopManager->m_nDDay;

	CreateDailyButton(MaxDay);

	if(OfflineHours<20)
	{
		SetIsEnabled(false);
		m_AryElements[m_nDDay]->Claimed();
	}
}

void UDailyRewardPanel::CreateDailyButton(int maxCount)
{
	check(m_ClassDailyElement);
	m_AryElements.Reserve(maxCount);

	int Index = 0;
	
	for(int y=0; y< 5; y++)
	{
		for(int x=0; x<5; x++)
		{
			UDailyRewardElement* CreatedCard = CreateWidget<UDailyRewardElement>(this, m_ClassDailyElement);

			auto* SlotUni = m_Grid->AddChildToUniformGrid(CreatedCard,y,x);//20//15

			SlotUni->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
			SlotUni->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);

			m_AryElements.Add(CreatedCard);

			CreatedCard->SetIsEnabled(false);

			CreatedCard->SetDailyRewardElement(Index+1,m_AryClaimGemstone[Index]);

			CreatedCard->m_BtnClaimReward->OnClicked.AddDynamic(this,&UDailyRewardPanel::ClaimReward);

			Index++;
		}		
	}

	m_AryElements[m_nDDay]->SetIsEnabled(true);
}

void UDailyRewardPanel::ClaimReward()
{
	m_AryElements[m_nDDay]->SetIsEnabled(false);
	m_AryElements[m_nDDay]->Claimed();
	UDiabloGameInstance::Get->m_ShopManager->ShowTouchBan();
	
	int GemStoneAmount = m_AryClaimGemstone[m_nDDay];

	m_nDDay++;
	
	if(m_nDDay>=25)
	{
		m_nDDay=0;
	}

	UDiabloGameInstance::Get->m_ShopManager->m_nDDay = m_nDDay; 
	
	UDiabloGameInstance::Get->m_QuestManager->AddGemStones(GemStoneAmount);
	UDiabloGameInstance::Get->m_QuestManager->RequestGemStoneUploadToServer();
	UDiabloGameInstance::Get->m_PlayfabManager->UploadIAPData();
}

//clami =>update need
//20hour prevent need;