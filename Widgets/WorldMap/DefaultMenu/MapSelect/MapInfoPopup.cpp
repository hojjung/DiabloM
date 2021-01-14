#include "MapInfoPopup.h"


#include "Characters/PlayerDiabloCharacter.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DiabloGameMode.h"
#include "Managers/DungeonManager.h"

#define LOCTEXT_NAMESPACE "DiaMapInfoPopup"

UImageAndText* UMapInfoPopup::CreateImageText(UTexture* texture, FText stringWant)
{
	UImageAndText* SlotCreated = CreateWidget<UImageAndText>(this, m_ClassImgText);

	SlotCreated->SetIcon(texture);
	SlotCreated->SetString(stringWant);
	m_AryImageTextCreated.Add(SlotCreated);
	return SlotCreated;
}

void UMapInfoPopup::Init()
{
	m_CurrentDgData = nullptr;
	m_DgManager = UDiabloGameInstance::Get->GetDungeonManager();

	m_BtnLeftMin->OnClicked.AddDynamic(this, &UMapInfoPopup::DecreaseMin);
	m_BtnRightMax->OnClicked.AddDynamic(this, &UMapInfoPopup::IncreaseMax);
	m_BtnRightIncrease->OnClicked.AddDynamic(this, &UMapInfoPopup::IncreaseDgLv);
	m_BtnLeftDecrease->OnClicked.AddDynamic(this, &UMapInfoPopup::DecreaseDgLv);
	//
	m_FormatMonsterLevel = LOCTEXT("MapPopupMobLevel", "Monster Level: {0}");
	m_FormatItemLevel = LOCTEXT("MapPopupMobItemLevel", "Dropable Item (Lv{0}~Lv{1})");
	m_FormatAutoPlay = LOCTEXT("MapPopupMobAutoPlay", "Auto Play In : {0}");
	//
	m_BtnClosePanel->OnClicked.AddDynamic(this, &UMapInfoPopup::ClosePopup);
	m_BtnEnterDg->OnClicked.AddDynamic(this, &UMapInfoPopup::EnterDungeon);
	m_BtnBackToVillage->OnClicked.AddDynamic(this, &UMapInfoPopup::PortalToVillage);
	//
	m_BtnCancelAutoStart->OnClicked.AddDynamic(this, &UMapInfoPopup::CancelCountdownAutoPlay);

	m_BtnCancelAutoStart->SetVisibility(ESlateVisibility::Hidden);

	m_fTimerMaxDelay = -1.f;

	m_ToggleAutoNext->OnCheckStateChanged.AddDynamic(this, &UMapInfoPopup::SetAutoNext);
	
	m_ToggleAutoRepeat->OnCheckStateChanged.AddDynamic(this, &UMapInfoPopup::SetAutoRepeat);
}

void UMapInfoPopup::SetMonsterAndItemLevel(const FDungeonDataRow* dg_data)
{
	int MonsterLevel = dg_data->StageLevelToDungeonLevel(m_nCurrentDgLevel);

	FFormatOrderedArguments Args;

	Args.Add(MonsterLevel);

	m_MonsterLevel->SetString(FText::Format(m_FormatMonsterLevel, Args));

	FFormatOrderedArguments Args2;

	int MinLevel = MonsterLevel - 3;
	int MaxLevel = MonsterLevel + 1;

	Args2.Add(FMath::Max(MinLevel, 1));
	Args2.Add(FMath::Min(MaxLevel,MAXLEVEL));

	m_DropItemLevel->SetString(FText::Format(m_FormatItemLevel, Args2));
}

void UMapInfoPopup::OpenPopup(const FDungeonDataRow* dg_data)
{
	if (m_CurrentDgData != dg_data)//던전 데이터 바뀌면 레벨 초기화
	{
		m_nCurrentDgLevel = 1;

		m_TextDgLevel->SetText(FText::AsNumber(m_nCurrentDgLevel));

		m_nMaxDgLevel = 100; //need fix
	}//정확히는 해당 던전 데이터가 가진 저장된 맥시멈을 가저와야함

	m_CurrentDgData = dg_data;

	if (!m_CurrentDgData)
	{
		PRINTF("MapInfo-No DgData");
		return;
	}

	//if(m_CurrentDgData->m_bIsInfinityDg)
	{
		float MaxLevel = m_CurrentDgData->m_DgLevelTable.GetMaxLevel();

		PRINTF("MaxLevel:%f",MaxLevel);
	}

	m_DgIcon->SetBrushFromTexture(m_CurrentDgData->m_DgIcon);

	m_DgName->SetText(m_CurrentDgData->m_DgShowName);

	SetMonsterAndItemLevel(m_CurrentDgData);


	//얼마나 느릴까 이함수는

	bool bIsAlreadyAdd = false;
	const FMonsterHordeHandle& Horde = m_CurrentDgData->m_Horde;
	for (const FMonsterSelect& MobSelect : Horde.GetRow<FMonsterHordeRow>("")->m_AryMonsterEntity)
	{
		for (const FItemDropData& DropItem : MobSelect.m_MonsterEntity.GetRow<FMonsterTable>("")->
		                                               m_RewardDropTableHandle.GetRow<FMonsterItemDropRow>("")->
		                                               m_AryDropItems)
		{
			const FItemData* Data = DropItem.m_DropHandle.GetRow<FItemData>("");

			m_AryItemData.Add(Data, &bIsAlreadyAdd);

			if (!bIsAlreadyAdd)
			{
				UImageAndText* Widget = CreateImageText(Data->m_ItemIcon, Data->m_ShowingName);

				m_VerticalInfo3->AddChildToVerticalBox(Widget);
			}

			m_AryItemTypes.Add(&Data->m_ItemType, &bIsAlreadyAdd);

			if (!bIsAlreadyAdd)
			{
				const FItemType* ItemTypeFound = Data->m_ItemType.GetRow<FItemType>("");

				UImageAndText* Widget = CreateImageText(ItemTypeFound->m_ItemTypeIcon, ItemTypeFound->m_ShowingName);

				m_VerticalInfo1->AddChildToVerticalBox(Widget);
			}
		}

		for (const FUniqueItemDropData& UniqueDropItem : MobSelect.m_MonsterEntity.GetRow<FMonsterTable>("")->
		                                                           m_RewardDropTableHandle.GetRow<FMonsterItemDropRow>(
			                                                           "")->m_AropDropUniqueItems)
		{
			const FUniqueEquipData* UniqueData = UniqueDropItem.m_DropHandle.GetRow<FUniqueEquipData>("");

			m_AryUniqueItemData.Add(UniqueData, &bIsAlreadyAdd);

			if (!bIsAlreadyAdd)
			{
				UImageAndText* Widget = CreateImageText(UniqueData->m_ItemIcon, UniqueData->m_ShowingName);

				m_VerticalInfo3->AddChildToVerticalBox(Widget);
			}

			m_AryItemTypes.Add(&UniqueData->m_ItemType, &bIsAlreadyAdd);

			if (!bIsAlreadyAdd)
			{
				const FItemType* ItemTypeFound = UniqueData->m_ItemType.GetRow<FItemType>("");

				UImageAndText* Widget = CreateImageText(ItemTypeFound->m_ItemTypeIcon, ItemTypeFound->m_ShowingName);

				m_VerticalInfo1->AddChildToVerticalBox(Widget);
			}
		}
	}

	//
	// for (const TSubclassOf<UDiabloAbility>& PlayerBuff : m_CurrentDgData->m_AryClassPlayerBuff)
	// {
	// 	UImageAndText* Widget = CreateImageText(PlayerBuff.GetDefaultObject()->m_AbilityIcon,
	// 	                                        PlayerBuff.GetDefaultObject()->m_ShowingName);
	//
	// 	//m_VerticalInfo2->InsertChildAt(m_VerticalInfo2->GetChildIndex(m_PlayerBuff)+1,Widget);
	// }
	//
	// for (const TSubclassOf<UDiabloAbility>& PlayerDebuff : m_CurrentDgData->m_AryClassPlayerDebuff)
	// {
	// 	UImageAndText* Widget = CreateImageText(PlayerDebuff.GetDefaultObject()->m_AbilityIcon,
	// 	                                        PlayerDebuff.GetDefaultObject()->m_ShowingName);
	//
	// 	//m_VerticalInfo2->InsertChildAt(m_VerticalInfo2->GetChildIndex(m_PlayerDebuff)+1,Widget);
	// }
	//
	// for (const TSubclassOf<UDiabloAbility>& MonsterBuff : m_CurrentDgData->m_AryClassMonsterBuff)
	// {
	// 	UImageAndText* Widget = CreateImageText(MonsterBuff.GetDefaultObject()->m_AbilityIcon,
	// 	                                        MonsterBuff.GetDefaultObject()->m_ShowingName);
	//
	// 	//m_VerticalInfo2->InsertChildAt(m_VerticalInfo2->GetChildIndex(m_MonsterBuff)+1,Widget);
	// }
	//
	// for (const TSubclassOf<UDiabloAbility>& MonsterDebuff : m_CurrentDgData->m_AryClassMonsterDebuff)
	// {
	// 	UImageAndText* Widget = CreateImageText(MonsterDebuff.GetDefaultObject()->m_AbilityIcon,
	// 	                                        MonsterDebuff.GetDefaultObject()->m_ShowingName);
	//
	// 	//m_VerticalInfo2->InsertChildAt(m_VerticalInfo2->GetChildIndex(m_MonsterDebuff)+1,Widget);
	// }
}

void UMapInfoPopup::SetAutoRepeat(bool b)
{
	if(b)
	{
		m_ToggleAutoNext->SetCheckedState(ECheckBoxState::Unchecked);
	}
}

void UMapInfoPopup::SetAutoNext(bool b)
{
	if(b)
	{
		m_ToggleAutoRepeat->SetCheckedState(ECheckBoxState::Unchecked);
	}
}


void UMapInfoPopup::ClosePopup()
{
	for (UImageAndText* WidgetVolatile : m_AryImageTextCreated)
	{
		WidgetVolatile->SetVisibility(ESlateVisibility::Collapsed);
	}

	m_AryImageTextCreated.Reset();
	m_AryItemData.Reset();
	m_AryItemTypes.Reset();
	m_AryUniqueItemData.Reset();

	//m_CurrentDgData=nullptr;

	SetVisibility(ESlateVisibility::Hidden);
}

void UMapInfoPopup::DecreaseDgLv() //몬스터에 차이가있는데 바뀔수있지
{
	m_nCurrentDgLevel--;

	if (m_nCurrentDgLevel < 1)
	{
		m_nCurrentDgLevel = 1;
	}

	m_TextDgLevel->SetText(FText::AsNumber(m_nCurrentDgLevel));
	SetMonsterAndItemLevel(m_CurrentDgData);
}

void UMapInfoPopup::IncreaseDgLv()
{
	m_nCurrentDgLevel++;

	if (m_nCurrentDgLevel > m_nMaxDgLevel)
	{
		m_nCurrentDgLevel = m_nMaxDgLevel;
	}

	m_TextDgLevel->SetText(FText::AsNumber(m_nCurrentDgLevel));
	SetMonsterAndItemLevel(m_CurrentDgData);
}

void UMapInfoPopup::DecreaseMin()
{
	m_nCurrentDgLevel -= 10;

	if (m_nCurrentDgLevel < 1)
	{
		m_nCurrentDgLevel = 1;
	}

	m_TextDgLevel->SetText(FText::AsNumber(m_nCurrentDgLevel));
	SetMonsterAndItemLevel(m_CurrentDgData);
}

void UMapInfoPopup::IncreaseMax()
{
	m_nCurrentDgLevel += 10;

	if (m_nCurrentDgLevel > m_nMaxDgLevel)
	{
		m_nCurrentDgLevel = m_nMaxDgLevel;
	}
	m_TextDgLevel->SetText(FText::AsNumber(m_nCurrentDgLevel));
	SetMonsterAndItemLevel(m_CurrentDgData);
}

void UMapInfoPopup::EnterDungeon()
{
	ClosePopup();
	m_DgManager->CreateDefaultInfinityDungeon(m_nCurrentDgLevel); //결국 던전 또한 레벨넘기는것으로 바껴야함? 동적 제작해도 되지않나

	bool Repeat = m_ToggleAutoRepeat->IsChecked();
	
	bool Next = m_ToggleAutoNext->IsChecked();

	if(Repeat || Next)
	{
		ADiabloPlayerController::Get->GetPlayerPawn()->SetAutoPlay(true);
	}
	PRINTF("EnterDungeon !");
}

void UMapInfoPopup::PortalToVillage()
{
	m_DgManager->PortalToVillage(true);
	TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
	DiaPC.Get()->GetMainCanvas()->CloseMapMenu();
}

void UMapInfoPopup::SetCountdownEnterDg(float wantDelay)
{
	m_fTimerMaxDelay = wantDelay;
}

void UMapInfoPopup::TryAutoEnter()
{
	bool Repeat = m_ToggleAutoRepeat->IsChecked();
	
	bool Next = m_ToggleAutoNext->IsChecked();

	if(!Repeat&&!Next)
	{
		PRINTF("MAPINFO -No Auto Setting,");
		return;
	}
	if(Next)
	{
		IncreaseDgLv();

		PRINTF("MAPINFO -NextStage");
	}
	
	PRINTF("MAPINFO -Auto Timer Stage On");
	
	SetCountdownEnterDg(4.f);
	
	m_BtnCancelAutoStart->SetVisibility(ESlateVisibility::Visible);
}

void UMapInfoPopup::CancelCountdownAutoPlay()
{
	m_fTimerMaxDelay= -1.f;
	
	m_BtnCancelAutoStart->SetVisibility(ESlateVisibility::Hidden);
}

void UMapInfoPopup::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(m_fTimerMaxDelay<=0.f)
	{
		return;
	}

	m_fTimerMaxDelay -= InDeltaTime;

	if(m_fTimerMaxDelay<=0.f)
	{
		EnterDungeon();

		m_fTimerMaxDelay=0.f;

		return;
	}
	
	FFormatOrderedArguments Args;

	FNumberFormattingOptions OO;
	OO.MaximumFractionalDigits =1;
	Args.Add(FText::AsNumber(m_fTimerMaxDelay,&OO));

	m_TextTimer->SetText(FText::Format(m_FormatAutoPlay, Args));
}
#undef LOCTEXT_NAMESPACE
