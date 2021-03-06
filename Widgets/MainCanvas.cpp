// My First Hack n Slash


#include "MainCanvas.h"


#include "Characters/PlayerDiabloCharacter.h"
#include "Engine/Canvas.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/MonsterSpawnManager.h"

void UMainCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//
	m_BtnQuest->OnClicked.AddDynamic(this,&UMainCanvas::SetActiveQuestPanel);
	m_BtnUpgrade->OnClicked.AddDynamic(this,&UMainCanvas::SetActiveUpgradePanel);
	m_BtnEquipment->OnClicked.AddDynamic(this,&UMainCanvas::SetActiveEquipmentPanel);
	m_BtnGacha->OnClicked.AddDynamic(this,&UMainCanvas::SetActiveGachaPanel);
	m_BtnShop->OnClicked.AddDynamic(this,&UMainCanvas::SetActiveShopPanel);
	m_BtnMenu->OnClicked.AddDynamic(this,&UMainCanvas::SetActiveMenuPanel);
	m_BtnBoss->OnClicked.AddDynamic(this,&UMainCanvas::SummonBoss);

	m_PanelQuest->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelUpgrade->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelEquipment->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelGacha->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelShop->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelMenu->SetVisibility(ESlateVisibility::Collapsed);

	UDiabloGameInstance::Get->m_GoldManager->m_OnGoldChanged.AddUObject(this,&UMainCanvas::UpdateGoldUI);
	UDiabloGameInstance::Get->m_MonsterSpawn->m_OnBossBattleEnd.AddUObject(this,&UMainCanvas::OnBossBattleEnd);
	UpdateGoldUI();

	m_fBossDurationTimeCounter=0.f;

	m_fMaxBossDurationTime = 30.f;

	m_fBossCooldownTimeCounter = 0.f;

	m_Format =FText::FromString("{0} Sec");

	HideBossUI();
	m_CDBoss->SetCooldownProgress(0,0);

	UDiabloGameInstance::Get->GetPlChar()->m_OnRageChanged.AddUObject(this,&UMainCanvas::UpdateRageBar);

}

void UMainCanvas::ReqeustText(FText txt)
{
	PlayAnimation(m_ShowText);
	
	m_PopupText->ShowText(txt);

	m_nIndex++;

	if(m_nIndex>=5)
	{
		m_nIndex=0;
	}
}

void UMainCanvas::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(m_fBossCooldownTimeCounter>0)
	{
		m_fBossCooldownTimeCounter-=InDeltaTime;
		
		m_CDBoss->SetCooldownProgress(m_fBossCooldownTimeCounter,m_fMaxBossCooldownTime);
	}

	if(!UDiabloGameInstance::Get->m_MonsterSpawn->IsBossBattleIn())
	{
		return;
	}

	m_fBossDurationTimeCounter+=InDeltaTime;

	float cTime = m_fMaxBossDurationTime - m_fBossDurationTimeCounter;
	UpdateTimer(cTime/m_fMaxBossDurationTime,cTime);

	if(m_fBossDurationTimeCounter>m_fMaxBossDurationTime)
	{
		UDiabloGameInstance::Get->m_MonsterSpawn->FailBossKill();
	}
}

void UMainCanvas::ShowBossUI()
{
	m_BtnBoss->SetVisibility(ESlateVisibility::Collapsed);
	m_OverlayTime->SetVisibility(ESlateVisibility::Visible);
	m_OverlayBossHp->SetVisibility(ESlateVisibility::Visible);
	//Set Timer
	//SetBossHp
	UpdateBossHP(1.f);
	UDiabloGameInstance::Get->m_MonsterSpawn->GetBossMob()->m_OnTookDmg.AddUObject(this,&UMainCanvas::UpdateBossHP);
	
}

void UMainCanvas::HideBossUI()
{
	m_BtnBoss->SetVisibility(ESlateVisibility::Visible);
	m_OverlayTime->SetVisibility(ESlateVisibility::Collapsed);
	m_OverlayBossHp->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainCanvas::UpdateRageBar(float cV,float mV)
{
	m_BarRage->SetProgressValue(cV/mV);

	FTextFormat FormatRage =FText::FromString("{0}/{1}");

	FFormatOrderedArguments Args;
	Args.Add(cV);
	Args.Add(mV);
	
	m_TxtRageValue->SetText(FText::Format(FormatRage,Args));
}

void UMainCanvas::UpdateBossHP(float per)
{
	m_BarBossHp->SetProgressValue(per);
	if(UDiabloGameInstance::Get->m_MonsterSpawn->GetBossMob())
	{
		if(UDiabloGameInstance::Get->m_MonsterSpawn->GetBossMob()->m_fCurrentHP.IsLessThanZero())
		{
			UpdateBossText(0);
		}
		else
		{
			UpdateBossText(UDiabloGameInstance::Get->m_MonsterSpawn->GetBossMob()->m_fCurrentHP);
		}
	}
}

void UMainCanvas::UpdateBossText(BigInt cHp)
{
	m_TxtBossHp->SetText(FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(cHp,2)));
}

void UMainCanvas::UpdateTimer(float per,float cTime)
{
	m_BarTime->SetProgressValue(per);

	FFormatOrderedArguments Args;

	Args.Add(UDiaBlueprintFunctionLibrary::SetFloatPrecision(cTime,1));

	m_TxtTime->SetText(FText::Format(m_Format,Args));
}

void UMainCanvas::SetBossTimer()
{
	m_fBossDurationTimeCounter = 0.f;
}

void UMainCanvas::SetActiveQuestPanel()
{
	if(m_PanelQuest->Visibility != ESlateVisibility::SelfHitTestInvisible)
	{
		m_PanelQuest->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_PanelQuest->SetVisibility(ESlateVisibility::Collapsed);
	}
	m_PanelUpgrade->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelEquipment->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelGacha->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelShop->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelMenu->SetVisibility(ESlateVisibility::Collapsed);	
}

void UMainCanvas::SetActiveUpgradePanel()
{
	m_PanelQuest->SetVisibility(ESlateVisibility::Collapsed);
	if(m_PanelUpgrade->Visibility != ESlateVisibility::SelfHitTestInvisible)
	{
		m_PanelUpgrade->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_PanelUpgrade->SetVisibility(ESlateVisibility::Collapsed);
	}
	m_PanelEquipment->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelGacha->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelShop->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelMenu->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainCanvas::SetActiveEquipmentPanel()
{
	m_PanelQuest->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelUpgrade->SetVisibility(ESlateVisibility::Collapsed);
	if(m_PanelEquipment->Visibility != ESlateVisibility::SelfHitTestInvisible)
	{
		m_PanelEquipment->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_PanelEquipment->SetVisibility(ESlateVisibility::Collapsed);
	}
	m_PanelGacha->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelShop->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelMenu->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainCanvas::SetActiveGachaPanel()
{
	m_PanelQuest->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelUpgrade->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelEquipment->SetVisibility(ESlateVisibility::Collapsed);
	if(m_PanelGacha->Visibility != ESlateVisibility::SelfHitTestInvisible)
	{
		m_PanelGacha->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_PanelGacha->SetVisibility(ESlateVisibility::Collapsed);
	}
	m_PanelShop->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelMenu->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainCanvas::SetActiveShopPanel()
{
	m_PanelQuest->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelUpgrade->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelEquipment->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelGacha->SetVisibility(ESlateVisibility::Collapsed);
	if(m_PanelShop->Visibility != ESlateVisibility::SelfHitTestInvisible)
	{
		m_PanelShop->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_PanelShop->SetVisibility(ESlateVisibility::Collapsed);
	}
	m_PanelMenu->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainCanvas::SetActiveMenuPanel()
{
	m_PanelQuest->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelUpgrade->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelEquipment->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelGacha->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelShop->SetVisibility(ESlateVisibility::Collapsed);
	if(m_PanelMenu->Visibility != ESlateVisibility::SelfHitTestInvisible)
	{
		m_PanelMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_PanelMenu->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMainCanvas::UpdateGoldUI()
{
	m_TextGold->SetText(FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(UDiabloGameInstance::Get->m_GoldManager->GetCurrentGold(),2)));
}

void UMainCanvas::SummonBoss()
{
	if(UDiabloGameInstance::Get->m_MonsterSpawn->IsBossBattleIn() || m_fBossCooldownTimeCounter>0)
	{
		return;
	}
	UDiabloGameInstance::Get->m_MonsterSpawn->SpawnBossMob();
	SetBossTimer();
	ShowBossUI();
	//시간초
}

void UMainCanvas::OnBossBattleEnd(bool b)
{
	UpdateBossText(0);
	HideBossUI();
	SetBossTimer();
	if(b)
	{
		PRINTF("BossSuccess");
		m_fMaxBossCooldownTime = 25.f;
	}
	else
	{
		PRINTF("BossFail");
		m_fMaxBossCooldownTime = 45.f;
	}

	
	m_CDBoss->StartCooldown();
	m_fBossCooldownTimeCounter = m_fMaxBossCooldownTime;
	//실패시 쿨타임
}
