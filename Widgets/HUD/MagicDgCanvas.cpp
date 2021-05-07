#include "MagicDgCanvas.h"
#include "Managers/DiabloGameInstance.h"

void UMagicDgCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UDiabloGameInstance::Get->m_MagicDgManager->m_OnTick.BindUObject(this,&UMagicDgCanvas::UpdateTimer);

	m_Handle = UDiabloGameInstance::Get->GetPlChar()->m_OnRageChanged.AddUObject(this,&UMagicDgCanvas::PlayerUpdateRageBar);
	
	UDiabloGameInstance::Get->GetPlChar()->UpdateRage();
	
	UDiabloGameInstance::Get->m_MagicDgManager->m_OnBattleEnd.BindUObject(this,&UMagicDgCanvas::OnBattleEnd);
	//
	UDiabloGameInstance::Get->m_MagicDgManager->m_OnDragonSpawned.BindUObject(this,&UMagicDgCanvas::OnDragonSpawned);
}


void UMagicDgCanvas::PlayerUpdateRageBar(float cV, float mV)
{
	float PercentOne = cV / mV;
	
	m_RageBar->SetProgressValue(PercentOne);
}

void UMagicDgCanvas::UpdateDragonHp(float percentOne)
{
	m_BarBossHp->SetProgressValue(percentOne);

	BigInt MobHp,MaxHp;
	
	UDiabloGameInstance::Get->m_MagicDgManager->m_SpawnedMagicDragon->GetHP(MobHp,MaxHp);
	
	m_TxtBossHp->SetText(FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(MobHp,2)));
}

void UMagicDgCanvas::OnDragonSpawned(AMonsterPawn* pawn)
{
	pawn->m_OnTookDmg.AddUObject(this,&UMagicDgCanvas::UpdateDragonHp);
}

void UMagicDgCanvas::UpdateTimer(float timer)
{
	float PercentOne = timer / MAGICDGTIME;

	m_BarTime->SetProgressValue(PercentOne);

	m_TxtTime->SetText(FText::FromString(FString::Printf(TEXT("남은 시간:%.1f"),timer)));
}

void UMagicDgCanvas::OnBattleEnd(bool isPlayerWon)
{
	PlayAnimation(ShowResult);

	int ObtainedBounty =0;
	
	if(isPlayerWon)
	{
		ObtainedBounty = UDiabloGameInstance::Get->m_MagicDgManager->GetResultBounty();
		
		m_TxtRequestedInfo->SetText(FText::FromString(FString::Printf(TEXT("성공-마정석 %d개 획득!"),ObtainedBounty)));	
	}
	else
	{
		ObtainedBounty = UDiabloGameInstance::Get->m_MagicDgManager->GetFailBounty();
		
		m_TxtRequestedInfo->SetText(FText::FromString(FString::Printf(TEXT("실패-마정석 %d개 획득.."),ObtainedBounty)));
	}
}