#include "WeaponDgCanvas.h"
#include "Managers/DiabloGameInstance.h"

void UWeaponDgCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_WeaponDgManager = UDiabloGameInstance::Get->m_WeaponDgManager;

	m_WeaponDgManager->m_OnTimerTick.AddUObject(this,&UWeaponDgCanvas::UpdateTimer);

	m_Handle = UDiabloGameInstance::Get->GetPlChar()->m_OnRageChanged.AddUObject(this,&UWeaponDgCanvas::PlayerUpdateRageBar);
	
	UDiabloGameInstance::Get->GetPlChar()->UpdateRage();
	
	m_WeaponDgManager->m_OnBattleEnd.AddUObject(this,&UWeaponDgCanvas::OnBattleEnd);

	m_WeaponDgManager->m_OnMobDead.BindUObject(this,&UWeaponDgCanvas::UpdateMobWeaponCount);
	//
	m_BarTime->SetProgressValue(1.f);
	
	UpdateMobWeaponCount(0,0);
}

void UWeaponDgCanvas::UpdateMobWeaponCount(int mobCount, int earnedStoneCount)
{
	m_TxtMobCount->SetText(FText::FromString(FString::Printf(TEXT("%d 처치"),mobCount)));
	
	m_TxtWStoneCount->SetText(FText::FromString(FString::Printf(TEXT("%d 획득"),earnedStoneCount)));
	
}

void UWeaponDgCanvas::UpdateTimer(float timer)
{
	float Per = 1.f - m_WeaponDgManager->GetTimePercent();
	
	m_BarTime->SetProgressValue(Per);

	m_TxtTime->SetText(FText::FromString(FString::Printf(TEXT("남은 시간:%.1f"),timer)));
}

void UWeaponDgCanvas::PlayerUpdateRageBar(float cV, float mV)
{
	float PercentOne = cV / mV;
	
	m_RageBar->SetProgressValue(PercentOne);

	m_TxtRageValue->SetText(FText::FromString(UDiabloGameInstance::Get->GetPlChar()->GetRageFormatStr()));
}

void UWeaponDgCanvas::OnBattleEnd(bool isPlayerWon)
{
	PlayAnimation(ShowResult);

	int ObtainedBounty = m_WeaponDgManager->GetObtainedStone();

	m_TxtRequestedInfo->SetText(FText::FromString(FString::Printf(TEXT("강화석 %d개 획득!"),ObtainedBounty)));
}
