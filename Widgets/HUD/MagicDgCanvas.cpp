#include "MagicDgCanvas.h"
#include "Managers/DiabloGameInstance.h"

void UMagicDgCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_MagicDgManager = UDiabloGameInstance::Get->m_MagicDgManager;

	m_MagicDgManager->m_OnTimerTick.AddUObject(this,&UMagicDgCanvas::UpdateTimer);

	m_Handle = UDiabloGameInstance::Get->GetPlChar()->m_OnRageChanged.AddUObject(this,&UMagicDgCanvas::PlayerUpdateRageBar);
	
	UDiabloGameInstance::Get->GetPlChar()->UpdateRage();
	
	m_MagicDgManager->m_OnBattleEnd.AddUObject(this,&UMagicDgCanvas::OnBattleEnd);
	//
	m_MagicDgManager->m_OnDragonSpawned.BindUObject(this,&UMagicDgCanvas::OnDragonSpawned);

	m_BarTime->SetProgressValue(1.f);
}


void UMagicDgCanvas::PlayerUpdateRageBar(float cV, float mV)
{
	float PercentOne = cV / mV;
	
	m_RageBar->SetProgressValue(PercentOne);

	m_TxtRageValue->SetText(FText::FromString(UDiabloGameInstance::Get->GetPlChar()->GetRageFormatStr()));
}

void UMagicDgCanvas::UpdateDragonHp(float percentOne)
{
	m_BarBossHp->SetProgressValue(percentOne);

	BigInt MobHp,MaxHp;
	
	m_MagicDgManager->m_SpawnedMagicDragon->GetHP(MobHp,MaxHp);
	
	m_TxtBossHp->SetText(FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(MobHp,2)));
}

void UMagicDgCanvas::OnDragonSpawned(AMonsterPawn* pawn)
{
	pawn->m_OnTookDmg.AddUObject(this,&UMagicDgCanvas::UpdateDragonHp);
	UpdateDragonHp(1.f);
}

void UMagicDgCanvas::UpdateTimer(float timer)
{
	float Per = 1.f - m_MagicDgManager->GetTimePercent();
	
	m_BarTime->SetProgressValue(Per);

	m_TxtTime->SetText(FText::FromString(FString::Printf(TEXT("남은 시간:%.1f"),timer)));
}

void UMagicDgCanvas::OnBattleEnd(bool isPlayerWon)
{
	PlayAnimation(ShowResult);

	int ObtainedBounty =0;
	
	if(isPlayerWon)
	{
		ObtainedBounty = m_MagicDgManager->GetResultBounty();
		
		m_TxtRequestedInfo->SetText(FText::FromString(FString::Printf(TEXT("성공-마정석 %d개 획득!"),ObtainedBounty)));	
	}
	else
	{
		ObtainedBounty = m_MagicDgManager->GetFailBounty();
		
		m_TxtRequestedInfo->SetText(FText::FromString(FString::Printf(TEXT("실패-마정석 %d개 획득.."),ObtainedBounty)));
	}
}