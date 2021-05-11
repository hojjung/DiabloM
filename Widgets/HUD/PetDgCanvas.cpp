// My First Hack n Slash


#include "PetDgCanvas.h"

#include "Managers/DiabloGameInstance.h"
#include "Widgets/CommonElement/MaterialProgressBar.h"


void UPetDgCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UDiabloGameInstance::Get->m_PetDgManager->m_OnTickTimer.AddUObject(this,&UPetDgCanvas::UpdateTimer);

	m_Handle = UDiabloGameInstance::Get->GetPlChar()->m_OnRageChanged.AddUObject(this,&UPetDgCanvas::PlayerUpdateRageBar);
	
	UDiabloGameInstance::Get->GetPlChar()->UpdateRage();
	
	UDiabloGameInstance::Get->m_PetDgManager->m_OnBattleEnd.AddUObject(this,&UPetDgCanvas::OnBattleEnd);

	UDiabloGameInstance::Get->m_PetDgManager->m_OnMonsterKilled.BindUObject(this,&UPetDgCanvas::UpdateMonsterCount);

	m_MonsterCountBar->SetProgressValue(0.f);
	
	m_TxtMonsterCountRemain->SetText(FText::FromString(TEXT("펫 사냥 시작")));
}

void UPetDgCanvas::UpdateTimer(float timer)
{
	float PercentOne = timer / PETDGTIME;

	m_BarTime->SetProgressValue(1.f-PercentOne);

	m_TxtTime->SetText(FText::FromString(FString::Printf(TEXT("남은 시간:%.1f"),PETDGTIME - timer)));
}

void UPetDgCanvas::OnBattleEnd(bool isPlayerWon)
{
	PlayAnimation(ShowResult);

	if(isPlayerWon)
	{
		int ObtainedBounty = UDiabloGameInstance::Get->m_PetDgManager->GetCurrentReward();
		
		FString RewardFormat = FString::Printf(TEXT("획득%d개"),ObtainedBounty);
		
		m_ImgTxtReward->SetText(FText::FromString(RewardFormat));
		
		m_TxtRequestedInfo->SetText(FText::FromString(TEXT("펫 던전 토벌 성공")));
	}
	else
	{
		m_TxtRequestedInfo->SetText(FText::FromString(TEXT("펫 던전 토벌 실패")));

		m_ImgTxtReward->SetText(FText::FromString(TEXT("획득불가")));
	}
}

void UPetDgCanvas::PlayerUpdateRageBar(float cV, float mV)
{
	float PercentOne = cV / mV;
	
	m_RageBar->SetProgressValue(PercentOne);
}

void UPetDgCanvas::UpdateMonsterCount()
{
	float PercentOne = UDiabloGameInstance::Get->m_PetDgManager->GetMobCountPercent();
	
	m_MonsterCountBar->SetProgressValue(PercentOne);
	
	m_TxtMonsterCountRemain->SetText(FText::FromString(UDiabloGameInstance::Get->m_PetDgManager->GetMobRemainCountStr()));
}


