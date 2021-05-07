// My First Hack n Slash


#include "DailyRewardPanel.h"
#include "UniformGridSlot.h"
#include "Managers/DiabloGameInstance.h"

void UDailyRewardPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	int MaxDay = UDiabloGameInstance::Get->m_DailyManager->m_AryClaimGemstone.Num();

	CreateDailyButton(MaxDay);
}

void UDailyRewardPanel::CreateDailyButton(int maxCount)
{
	check(m_ClassDailyElement);
	
	m_AryElements.Reserve(maxCount);

	int Index = 0;

	int DDay = UDiabloGameInstance::Get->m_DailyManager->m_nDDay;
	
	for(int y=0; y< 5; y++)
	{
		for(int x=0; x<5; x++)
		{
			UDailyRewardElement* CreatedCard = CreateWidget<UDailyRewardElement>(this, m_ClassDailyElement);

			auto* SlotUni = m_Grid->AddChildToUniformGrid(CreatedCard,y,x);//20//15

			SlotUni->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
			SlotUni->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);

			m_AryElements.Add(CreatedCard);

			CreatedCard->SetDailyRewardElement(Index+1,UDiabloGameInstance::Get->m_DailyManager->m_AryClaimGemstone[Index]);

			if(Index<DDay)
			{
				CreatedCard->SetIsEnabled(false);
			}

			Index++;
		}		
	}

	auto* CurrentElement = m_AryElements[DDay];
	
	if(UDiabloGameInstance::Get->m_DailyManager->m_bIsAbleGetDailyPrize)
	{
		CurrentElement->SetClaimAble();
		CurrentElement->m_BtnClaimReward->OnClicked.AddDynamic(this,&UDailyRewardPanel::ClaimReward);	
	}
	else
	{
		CurrentElement->m_bIsAvailable=true;
		CurrentElement->Claimed();
	}
}

void UDailyRewardPanel::ClaimReward()
{
	PRINTF("DailyRewardPanel");
	UDiabloGameInstance::Get->m_DailyManager->Claim();
}

//clami =>update need
//20hour prevent need;