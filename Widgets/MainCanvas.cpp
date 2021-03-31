#include "MainCanvas.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Engine/Canvas.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/MonsterSpawnManager.h"
#include "Shopmenu/ShopPanel.h"


#define LOCTEXT_NAMESPACE "MainCanvas"

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

	m_fMaxBossDurationTime = 12.f;

	m_fBossCooldownTimeCounter = 0.f;

	m_Format =FText::FromString("{0} Sec");

	HideBossUI();
	m_CDBoss->SetCooldownProgress(0,0);

	UDiabloGameInstance::Get->GetPlChar()->m_OnRageChanged.AddUObject(this,&UMainCanvas::UpdateRageBar);

	SetPlayerNicknameRanking();

	UDiabloGameInstance::Get->m_ShopManager->m_OnShowAdBanner.AddUObject(this,&UMainCanvas::OnShowAdBanner);
	UDiabloGameInstance::Get->m_PlayfabManager->m_OnGemstoneChanged.AddUObject(this,&UMainCanvas::UpdateGemStone);
	UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	UDiabloGameInstance::Get->m_PlayfabManager->m_OnPlayerRankReceived.AddUObject(this,&UMainCanvas::SetPlayerNicknameRankingWrap);

	m_BtnGemStone->OnClicked.AddDynamic(this,&UMainCanvas::OpenGemStoneShop);
	m_BtnGold->OnClicked.AddDynamic(this,&UMainCanvas::OpenGoldShop);
	//
	UDiabloGameInstance::Get->m_ShopManager->ShowBannerAD(true);

	m_GameExitWindow->Cancel();
	
	m_OfflineGoldPanel->SetVisibility(ESlateVisibility::Collapsed);

	OnTouchBanHide();
}

void UMainCanvas::ShowTouchBan(float secWant)
{
	GetWorld()->GetTimerManager().ClearTimer(m_TouchBanTimer);
	
	m_TouchBan->SetVisibility(ESlateVisibility::Visible);
	//
	GetWorld()->GetTimerManager().SetTimer(m_TouchBanTimer, this, &UMainCanvas::OnTouchBanHide,secWant, false);
}

void UMainCanvas::OnTouchBanHide()
{
	m_TouchBan->SetVisibility(ESlateVisibility::Collapsed);
}


void UMainCanvas::RequestText(FText txt)
{
	m_PopupText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	PlayAnimation(m_ShowText);

	m_PopupText->ShowText(txt);
}

void UMainCanvas::RequestText(FString txt)
{
	RequestText(FText::FromString(txt));
}

void UMainCanvas::ShowOfflineGoldWindow(BigInt gold)
{
	m_OfflineGoldPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	m_OfflineGoldPanel->SetOfflineGold(gold);
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
	int Erase = cV;
	
	cV =Erase;
	
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
//UDiabloGameInstance::Get->m_PlayfabManager->ShowBannerAd(false);
void UMainCanvas::SetActiveQuestPanel()
{
	if(m_PanelQuest->Visibility != ESlateVisibility::SelfHitTestInvisible)
	{
		m_PanelQuest->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		OnMenuPanelVisibleChanged(true);
	}
	else
	{
		m_PanelQuest->SetVisibility(ESlateVisibility::Collapsed);
		
		OnMenuPanelVisibleChanged(false);
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
		
		OnMenuPanelVisibleChanged(true);
	}
	else
	{
		m_PanelUpgrade->SetVisibility(ESlateVisibility::Collapsed);
		
		OnMenuPanelVisibleChanged(false);
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
		
		OnMenuPanelVisibleChanged(true);
	}
	else
	{
		m_PanelEquipment->SetVisibility(ESlateVisibility::Collapsed);
		
		OnMenuPanelVisibleChanged(false);
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

		OnMenuPanelVisibleChanged(true);
	}
	else
	{
		m_PanelGacha->SetVisibility(ESlateVisibility::Collapsed);
		
		OnMenuPanelVisibleChanged(false);
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
		
		OnMenuPanelVisibleChanged(true);
	}
	else
	{
		m_PanelShop->SetVisibility(ESlateVisibility::Collapsed);
		
		OnMenuPanelVisibleChanged(false);
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

		OnMenuPanelVisibleChanged(true);
	}
	else
	{
		m_PanelMenu->SetVisibility(ESlateVisibility::Collapsed);
		
		OnMenuPanelVisibleChanged(false);
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
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Success Boss Kill","Success Boss Kill"));
		m_fMaxBossCooldownTime = 25.f;
	}
	else
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("Fail Boss Kill","Fail Boss Kill"));
		m_fMaxBossCooldownTime = 45.f;
	}

	
	m_CDBoss->StartCooldown();
	m_fBossCooldownTimeCounter = m_fMaxBossCooldownTime;
	//실패시 쿨타임
}

void UMainCanvas::SetPlayerNicknameRanking()
{
	FFormatOrderedArguments Args;

	int Rank = UDiabloGameInstance::Get->m_PlayfabManager->GetRanking();
	
	Args.Add(Rank);
	Args.Add(FText::FromString(UDiabloGameInstance::Get->m_PlayfabManager->m_LoadedNickname));

	FTextFormat Format = LOCTEXT("Rank Nickname","[{0}st] {1}");
	
	FText RankText = FText::Format(Format,Args);
	
	m_TextRanking->SetText(RankText);
}

void UMainCanvas::SetPlayerNicknameRankingWrap(const TArray<PlayFab::ClientModels::FPlayerLeaderboardEntry>& notUse)
{
	SetPlayerNicknameRanking();
}

void UMainCanvas::HideTextWidget()
{
	m_PopupText->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainCanvas::OnAnimationFinishedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationFinishedPlaying(Player);

	HideTextWidget();
}

void UMainCanvas::UpdateGemStone(int v)
{
	m_TextGemStone->SetText(FText::AsNumber(v));
}

void UMainCanvas::OpenGoldShop()
{
	SetActiveShopPanel();
	m_PanelShop->ShowGoldPanel();
}

void UMainCanvas::OpenGemStoneShop()
{
	SetActiveShopPanel();
	m_PanelShop->ShowGemStonePanel();
}

void UMainCanvas::OnShowAdBanner(bool b)
{
	if(b)
	{
		Cast<UCanvasPanelSlot>( m_CanvasBannerAds->Slot)->SetPosition(FVector2D(0,150.f));
	}
	else
	{
		Cast<UCanvasPanelSlot>( m_CanvasBannerAds->Slot)->SetPosition(FVector2D(0,0));
	}
}

void UMainCanvas::OnPressBackkey()
{
	m_GameExitWindow->OnBackkeyPressed();
}

void UMainCanvas::OnMenuPanelVisibleChanged(bool b)
{
	
	
	UDiabloGameInstance::Get->m_ShopManager->ShowBannerAD(!b);

	m_OnMenuVisibleChanged.Broadcast(b);
}

#undef LOCTEXT_NAMESPACE
