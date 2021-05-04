#include "PVPCanvas.h"
#include "CommonElement/MaterialProgressBar.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Managers/DiabloGameInstance.h"

void UPVPCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UDiabloGameInstance::Get->m_PVPManager->m_OnDmgChanged.BindUObject(this,&UPVPCanvas::UpdateDmgGauge);
	
	m_TxtPlayerName->SetText(FText::FromString(UDiabloGameInstance::Get->m_PlayfabManager->m_LoadedNickname));
	
	m_TxtOtherPlayerName->SetText(FText::FromString(UDiabloGameInstance::Get->m_PVPManager->m_OtherPlayerDisplayName));

	UDiabloGameInstance::Get->m_PVPManager->m_OnTick.BindUObject(this,&UPVPCanvas::UpdateTimer);

	UDiabloGameInstance::Get->m_PVPManager->m_OnBattleEnd.BindUObject(this,&UPVPCanvas::OnBattleEnd);

	OnEquipSkillSet(UDiabloGameInstance::Get->m_PVPManager->m_PVPOtherPlayer.Get()->GetEquippedSkill());

	UDiabloGameInstance::Get->GetPlChar()->m_OnRageChanged.AddUObject(this,&UPVPCanvas::PlayerUpdateRageBar);

	UDiabloGameInstance::Get->m_PVPManager->m_PVPOtherPlayer.Get()->m_OnRageChanged.AddUObject(this,&UPVPCanvas::OtherPlayerUpdateRageBar);

	UDiabloGameInstance::Get->m_PVPManager->m_PVPOtherPlayer.Get()->UpdateRage();

	UDiabloGameInstance::Get->GetPlChar()->UpdateRage();

}

void UPVPCanvas::UpdateDmgGauge(float percentOne, BigInt playerDmg, BigInt otherPlayerDmg)
{
	m_BarPlayerDmg->SetProgressValue(percentOne);
	
	m_TxtPlayerDmgValue->SetText(FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(playerDmg,2)));
	m_TxtOtherPlayerDmgValue->SetText(FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(otherPlayerDmg,2)));
}

void UPVPCanvas::ReqeustPopupText(const FText& str)
{
	m_TxtRequestedInfo->SetText(str);
}

void UPVPCanvas::UpdateTimer(float timer)
{
	m_TxtTimer->SetText(FText::FromString(FString::Printf(TEXT("남은시간:%.1f"),timer)));
}

void UPVPCanvas::OnBattleEnd(bool isPlayerWon)
{
	PlayAnimation(ShowResult);
	
	if(isPlayerWon)
	{
		m_TxtRequestedInfo->SetText(FText::FromString(TEXT("승리")));	
	}
	else
	{
		m_TxtRequestedInfo->SetText(FText::FromString(TEXT("패배")));
	}
	
}

void UPVPCanvas::OnEquipSkillSet(TArray<FSkillSpec*>& equippedSkill)
{
	m_OtherPlayerSkill01->Init(0);
	
	m_OtherPlayerSkill02->Init(1);
	m_OtherPlayerSkill03->Init(2);
	m_OtherPlayerSkill04->Init(3);

	if(equippedSkill[0]->m_SkillData)
	{
		m_OtherPlayerSkill01->SetSkillSpec(equippedSkill[0]);	
	}

	if(equippedSkill[1]->m_SkillData)
	{
		m_OtherPlayerSkill02->SetSkillSpec(equippedSkill[1]);	
	}

	if(equippedSkill[2]->m_SkillData)
	{
		m_OtherPlayerSkill03->SetSkillSpec(equippedSkill[2]);	
	}

	if(equippedSkill[3]->m_SkillData)
	{
		m_OtherPlayerSkill04->SetSkillSpec(equippedSkill[3]);	
	}

	m_OtherPlayerSkill01->m_bIsSkillUsable=false;
	m_OtherPlayerSkill02->m_bIsSkillUsable=false;
	m_OtherPlayerSkill03->m_bIsSkillUsable=false;
	m_OtherPlayerSkill04->m_bIsSkillUsable=false;
}

void UPVPCanvas::PlayerUpdateRageBar(float cV, float mV)
{
	int Erase = cV;
	
	cV =Erase;
	
	m_PlayerBarRage->SetProgressValue(cV/mV);

	FTextFormat FormatRage =FText::FromString(TEXT("분노 {0}/{1}"));

	FFormatOrderedArguments Args;
	Args.Add(cV);
	Args.Add(mV);
	
	m_TxtPlayerRageValue->SetText(FText::Format(FormatRage,Args));	
}

void UPVPCanvas::OtherPlayerUpdateRageBar(float cV, float mV)
{
	int Erase = cV;
	
	cV =Erase;
	
	m_OtherPlayerBarRage->SetProgressValue(cV/mV);

	FTextFormat FormatRage =FText::FromString(TEXT("분노 {0}/{1}"));

	FFormatOrderedArguments Args;
	Args.Add(cV);
	Args.Add(mV);
	
	m_TxtOtherPlayerRageValue->SetText(FText::Format(FormatRage,Args));	
}
