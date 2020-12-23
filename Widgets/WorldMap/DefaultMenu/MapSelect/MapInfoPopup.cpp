#include "MapInfoPopup.h"
#include "Managers/DiabloGameInstance.h"
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
	m_CurrentDgData=nullptr;
	m_DgManager = UDiabloGameInstance::Get->GetDungeonManager();

	m_BtnLeftMin->OnClicked.AddDynamic(this,&UMapInfoPopup::DecreaseMin);
	m_BtnRightMax->OnClicked.AddDynamic(this,&UMapInfoPopup::IncreaseMax);
	m_BtnRightIncrease->OnClicked.AddDynamic(this,&UMapInfoPopup::IncreaseDgLv);
	m_BtnLeftDecrease->OnClicked.AddDynamic(this,&UMapInfoPopup::DecreaseDgLv);
	//
	m_FormatMonsterLevel = LOCTEXT("MapPopupMobLevel","Monster Level: {0}");
	m_FormatItemLevel = LOCTEXT("MapPopupMobItemLevel","Dropable Item (Lv{0}~Lv{1})");
	//
	m_BtnClosePanel->OnClicked.AddDynamic(this,&UMapInfoPopup::ClosePopup);
	m_BtnEnterDg->OnClicked.AddDynamic(this,&UMapInfoPopup::EnterDungeon);
	m_BtnBackToVillage->OnClicked.AddDynamic(this,&UMapInfoPopup::PortalToVillage);
}

void UMapInfoPopup::SetMonsterAndItemLevel(const FDungeonDataRow* dg_data)
{
	int MonsterLevel = dg_data->StageLevelToDungeonLevel(m_nCurrentDgLevel);

	FFormatOrderedArguments Args;

	Args.Add(MonsterLevel);
	
	m_MonsterLevel->SetString(FText::Format(m_FormatMonsterLevel,Args));

	FFormatOrderedArguments Args2;

	int MinLevel =MonsterLevel-3;
	int MaxLevel =MonsterLevel+1;
	
	Args2.Add(FMath::Max(MinLevel,1));
	Args2.Add(FMath::Min(MaxLevel,MAXLEVEL));

	m_DropItemLevel->SetString(FText::Format(m_FormatItemLevel,Args2));
}

void UMapInfoPopup::OpenPopup(const FDungeonDataRow* dg_data)
{
	if(m_CurrentDgData!=dg_data)
	{
		m_nCurrentDgLevel=1;

		m_TextDgLevel->SetText(FText::AsNumber(m_nCurrentDgLevel));
	
		m_nMaxDgLevel=100;//need fix
	}
	
	m_CurrentDgData=dg_data;

	if(!m_CurrentDgData)
	{
		PRINTF("MapInfo-No DgData");
		return;
	}
	
	
	
	m_DgIcon->SetBrushFromTexture(m_CurrentDgData->m_DgIcon);
	
	m_DgName->SetText(m_CurrentDgData->m_DgShowName);
	
	SetMonsterAndItemLevel(m_CurrentDgData);
	

	//얼마나 느릴까 이함수는

	bool bIsAlreadyAdd=false;
	for(const FMonsterHordeHandle& Horde : m_CurrentDgData->m_AryHorde)
	{
		for(const FMonsterSelect& MobSelect : Horde.GetRow<FMonsterHordeRow>("")->m_AryMonsterEntity)
		{
			for(const FItemDropData& DropItem : MobSelect.m_MonsterEntity.GetRow<FMonsterTable>("")->m_RewardDropTableHandle.GetRow<FMonsterItemDropRow>("")->m_AryDropItems)
			{
				const FItemData* Data = DropItem.m_DropHandle.GetRow<FItemData>("");

				m_AryItemData.Add(Data,&bIsAlreadyAdd);

				if(!bIsAlreadyAdd)
				{
					UImageAndText* Widget = CreateImageText(Data->m_ItemIcon, Data->m_ShowingName);

					m_VerticalInfo3->AddChildToVerticalBox(Widget);
				}

				m_AryItemTypes.Add(&Data->m_ItemType,&bIsAlreadyAdd);
				
				if(!bIsAlreadyAdd)
				{
					const FItemType* ItemTypeFound =Data->m_ItemType.GetRow<FItemType>("");
					
					UImageAndText* Widget = CreateImageText(ItemTypeFound->m_ItemTypeIcon, ItemTypeFound->m_ShowingName);
					
					m_VerticalInfo1->AddChildToVerticalBox(Widget);
				}
			}

			for(const FUniqueItemDropData& UniqueDropItem : MobSelect.m_MonsterEntity.GetRow<FMonsterTable>("")->m_RewardDropTableHandle.GetRow<FMonsterItemDropRow>("")->m_AropDropUniqueItems)
			{
				const FUniqueEquipData* UniqueData = UniqueDropItem.m_DropHandle.GetRow<FUniqueEquipData>("");

				m_AryUniqueItemData.Add(UniqueData,&bIsAlreadyAdd);

				if(!bIsAlreadyAdd)
				{
					UImageAndText* Widget = CreateImageText(UniqueData->m_ItemIcon, UniqueData->m_ShowingName);

					m_VerticalInfo3->AddChildToVerticalBox(Widget);
				}

				m_AryItemTypes.Add(&UniqueData->m_ItemType,&bIsAlreadyAdd);
				
				if(!bIsAlreadyAdd)
				{
					const FItemType* ItemTypeFound =UniqueData->m_ItemType.GetRow<FItemType>("");
					
					UImageAndText* Widget = CreateImageText(ItemTypeFound->m_ItemTypeIcon, ItemTypeFound->m_ShowingName);
					
					m_VerticalInfo1->AddChildToVerticalBox(Widget);
				}
			}
		}
	}

	 for(const TSubclassOf<UDiabloAbility>& PlayerBuff : m_CurrentDgData->m_AryClassPlayerBuff)
	 {
	 	UImageAndText* Widget = CreateImageText(PlayerBuff.GetDefaultObject()->m_AbilityIcon,PlayerBuff.GetDefaultObject()->m_ShowingName);

	 	//m_VerticalInfo2->InsertChildAt(m_VerticalInfo2->GetChildIndex(m_PlayerBuff)+1,Widget);
	 }

	for(const TSubclassOf<UDiabloAbility>& PlayerDebuff : m_CurrentDgData->m_AryClassPlayerDebuff)
	{
		UImageAndText* Widget = CreateImageText(PlayerDebuff.GetDefaultObject()->m_AbilityIcon,PlayerDebuff.GetDefaultObject()->m_ShowingName);

		//m_VerticalInfo2->InsertChildAt(m_VerticalInfo2->GetChildIndex(m_PlayerDebuff)+1,Widget);
	}

	for(const TSubclassOf<UDiabloAbility>& MonsterBuff : m_CurrentDgData->m_AryClassMonsterBuff)
	{
		UImageAndText* Widget = CreateImageText(MonsterBuff.GetDefaultObject()->m_AbilityIcon,MonsterBuff.GetDefaultObject()->m_ShowingName);

		//m_VerticalInfo2->InsertChildAt(m_VerticalInfo2->GetChildIndex(m_MonsterBuff)+1,Widget);
	}

	for(const TSubclassOf<UDiabloAbility>& MonsterDebuff : m_CurrentDgData->m_AryClassMonsterDebuff)
	{
		UImageAndText* Widget = CreateImageText(MonsterDebuff.GetDefaultObject()->m_AbilityIcon,MonsterDebuff.GetDefaultObject()->m_ShowingName);

		//m_VerticalInfo2->InsertChildAt(m_VerticalInfo2->GetChildIndex(m_MonsterDebuff)+1,Widget);
	}

	
}


void UMapInfoPopup::ClosePopup()
{
	for(UImageAndText* WidgetVolatile : m_AryImageTextCreated)
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

	if(m_nCurrentDgLevel<1)
	{
		m_nCurrentDgLevel=1;
	}

	m_TextDgLevel->SetText(FText::AsNumber(m_nCurrentDgLevel));
	SetMonsterAndItemLevel(m_CurrentDgData);
}

void UMapInfoPopup::IncreaseDgLv()
{
	m_nCurrentDgLevel++;

	if(m_nCurrentDgLevel>m_nMaxDgLevel)
	{
		m_nCurrentDgLevel=m_nMaxDgLevel;
	}

	m_TextDgLevel->SetText(FText::AsNumber(m_nCurrentDgLevel));
	SetMonsterAndItemLevel(m_CurrentDgData);
}

void UMapInfoPopup::DecreaseMin()
{
	m_nCurrentDgLevel-=10;

	if(m_nCurrentDgLevel<1)
	{
		m_nCurrentDgLevel=1;
	}
	
	m_TextDgLevel->SetText(FText::AsNumber(m_nCurrentDgLevel));
	SetMonsterAndItemLevel(m_CurrentDgData);
}

void UMapInfoPopup::IncreaseMax()
{
	m_nCurrentDgLevel+=10;

	if(m_nCurrentDgLevel>m_nMaxDgLevel)
	{
		m_nCurrentDgLevel=m_nMaxDgLevel;
	}
	m_TextDgLevel->SetText(FText::AsNumber(m_nCurrentDgLevel));
	SetMonsterAndItemLevel(m_CurrentDgData);
}

void UMapInfoPopup::EnterDungeon()
{
	ClosePopup();
	m_DgManager->CreateDefaultInfinityDungeon(m_nCurrentDgLevel);//결국 던전 또한 레벨넘기는것으로 바껴야함? 동적 제작해도 되지않나
}

void UMapInfoPopup::PortalToVillage()
{
	m_DgManager->PortalToVillage(true);
	TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
	DiaPC.Get()->CloseMapSelectMenu();
	
}
#undef LOCTEXT_NAMESPACE

