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

	m_nBeforeMagicStones = UDiabloGameInstance::Get->m_GoldManager->GetSkillStone();
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
		
		m_TxtRequestedInfo->SetText(FText::FromString(FString::Printf(TEXT("성공"))));

		m_ImgTxtRemainTime->SetText(FText::FromString(FString::Printf(TEXT("남은시간:%.1f"),m_MagicDgManager->GetRemainTimer())));
	}
	else
	{
		ObtainedBounty = m_MagicDgManager->GetFailBounty();
		
		m_TxtRequestedInfo->SetText(FText::FromString(FString::Printf(TEXT("실패"))));
		
		m_ImgTxtRemainTime->SetText(FText::FromString(FString::Printf(TEXT("시간초과"))));
	}

	m_ImgTxtBeforeSkillStone->SetText(FText::FromString(FString::Printf(TEXT("이전 마정석:%d"),m_nBeforeMagicStones)));
	
	m_ImgTxtReward->SetText(FText::FromString(FString::Printf(TEXT("얻은 마정석:%d"),ObtainedBounty)));

	int FinalMagicStones = UDiabloGameInstance::Get->m_GoldManager->GetSkillStone();

	m_ImgTxtAfterSkillStone->SetText(FText::FromString(FString::Printf(TEXT("현재 마정석:%d"),FinalMagicStones)));
}