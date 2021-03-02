// My First Hack n Slash


#include "MainCanvas.h"

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

	m_fTimeCounter=0.f;

	m_fMaxBossTime = 30.f;

	m_Format =FText::FromString("{0} Sec");

	HideBossUI();
}

void UMainCanvas::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!UDiabloGameInstance::Get->m_MonsterSpawn->IsBossBattleIn())
	{
		return;
	}

	m_fTimeCounter+=InDeltaTime;

	float cTime = m_fMaxBossTime - m_fTimeCounter;
	UpdateTimer(cTime/m_fMaxBossTime,cTime);

	if(m_fTimeCounter>m_fMaxBossTime)
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

void UMainCanvas::UpdateBossHP(float per)
{
	m_BarBossHp->SetProgressValue(per);
	if(UDiabloGameInstance::Get->m_MonsterSpawn->GetBossMob())
	{
		if(UDiabloGameInstance::Get->m_MonsterSpawn->GetBossMob()->m_fCurrentHP.IsLessThanZero())
		{
			UpdateBossText(0);
			HideBossUI();
			SetBossTimer();
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
	m_fTimeCounter = 0.f;
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
	if(UDiabloGameInstance::Get->m_MonsterSpawn->IsBossBattleIn())
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
	
	
	if(b)
	{
		PRINTF("BossSuccess");
		//성공
		return;
	}

	PRINTF("BossFail");

	//실패시 쿨타임
}
