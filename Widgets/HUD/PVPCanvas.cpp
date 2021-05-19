#include "PVPCanvas.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Managers/DiabloGameInstance.h"
#include "Widgets/CommonElement/MaterialProgressBar.h"

void UPVPCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UDiabloGameInstance::Get->m_PVPManager->m_OnDmgChanged.BindUObject(this,&UPVPCanvas::UpdateDmgGauge);
	
	m_TxtPlayerName->SetText(FText::FromString(UDiabloGameInstance::Get->m_PlayfabManager->m_LoadedNickname));
	
	m_TxtOtherPlayerName->SetText(FText::FromString(UDiabloGameInstance::Get->m_PVPManager->m_OtherPlayerDisplayName));

	UDiabloGameInstance::Get->m_PVPManager->m_OnTimerTick.AddUObject(this,&UPVPCanvas::UpdateTimer);

	UDiabloGameInstance::Get->m_PVPManager->m_OnBattleEnd.AddUObject(this,&UPVPCanvas::OnBattleEnd);

	m_Handle = UDiabloGameInstance::Get->GetPlChar()->m_OnRageChanged.AddUObject(this,&UPVPCanvas::PlayerUpdateRageBar);
	//
	UDiabloGameInstance::Get->m_PVPManager->m_OnOtherPlayerSpawned.BindUObject(this,&UPVPCanvas::OnOtherPlayerSpawned);

	UDiabloGameInstance::Get->GetPlChar()->UpdateRage();

	m_BeforeMMR = UDiabloGameInstance::Get->m_PVPManager->GetMMR(); 

	UDiabloGameInstance::Get->m_PVPManager->m_OnPVPStatusChanged.AddUObject(this,&UPVPCanvas::UpdatePVPResult);
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

	FSafeInt Reward = 3;

	if(isPlayerWon)
	{
		m_TxtRequestedInfo->SetText(FText::FromString(TEXT("승리")));
	}
	else
	{
		m_TxtRequestedInfo->SetText(FText::FromString(TEXT("패배")));

		Reward =1;
	}
	
	m_ImgTxtReward->SetText(FText::FromString(FString::Printf(TEXT("날개 교환권 %d개 획득"),Reward.GetValue())));
}

void UPVPCanvas::OnEquipSkillSet(TArray<FSkillSpec*>& equippedSkill)
{
	m_OtherPlayerSkill01->Init(0);
	
	m_OtherPlayerSkill02->Init(1);
	m_OtherPlayerSkill03->Init(2);
	m_OtherPlayerSkill04->Init(3);

	if(equippedSkill[0])
	{
		m_OtherPlayerSkill01->SetSkillSpec(equippedSkill[0]);	
	}

	if(equippedSkill[1])
	{
		m_OtherPlayerSkill02->SetSkillSpec(equippedSkill[1]);	
	}

	if(equippedSkill[2])
	{
		m_OtherPlayerSkill03->SetSkillSpec(equippedSkill[2]);	
	}

	if(equippedSkill[3])
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

	m_TxtPlayerRageValue->SetText(FText::FromString(UDiabloGameInstance::Get->GetPlChar()->GetRageFormatStr()));
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

void UPVPCanvas::OnOtherPlayerSpawned(AOtherPlayerPawn* pawn)
{
	OnEquipSkillSet(pawn->GetEquippedSkill());

	pawn->m_OnRageChanged.AddUObject(this,&UPVPCanvas::OtherPlayerUpdateRageBar);

	pawn->UpdateRage();
}

void UPVPCanvas::UpdatePVPResult(int w, int l , int mmr)
{
	int MMRIDff = mmr - m_BeforeMMR;
	int Reward = UDiabloGameInstance::Get->m_PVPManager->m_nCurrentWingTicket;

	m_ImgTxtWinLose->SetText(FText::FromString(FString::Printf(TEXT("승:%d,패:%d"),w,l)));
	m_ImgTxtReward->SetText(FText::FromString(FString::Printf(TEXT("획득 날개티켓:%d"),Reward)));
	m_ImgTxtMMRBefore->SetText(FText::FromString(FString::Printf(TEXT("이전 MMR:%d"),m_BeforeMMR)));
	m_ImgTxtMMRGained->SetText(FText::FromString(FString::Printf(TEXT("획득 MMR:%d"),MMRIDff)));
	m_ImgTxtMMRAfter->SetText(FText::FromString(FString::Printf(TEXT("현재 MMR:%d"),mmr)));
}