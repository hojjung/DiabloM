#include "MapSelectPanel.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DungeonManager.h"

UMapSelectPanel::UMapSelectPanel(const FObjectInitializer& objInit):Super(objInit)
{
	
}

void UMapSelectPanel::NativePreConstruct()
{
	Super::NativePreConstruct();

	InitDgButton(m_DefaultDgHandle.GetRow<FDungeonDataRow>("NoDgData1"),m_BtnDefaultInf);
	InitDgButton(m_DemonDgHandle.GetRow<FDungeonDataRow>("NoDgData2"),m_BtnDemon);
	InitDgButton(m_BeastDgHandle.GetRow<FDungeonDataRow>("NoDgData3"),m_BtnBeast);
	InitDgButton(m_UndeadDgHandle.GetRow<FDungeonDataRow>("NoDgData4"),m_BtnUndead);
	InitDgButton(m_HordeDgHandle.GetRow<FDungeonDataRow>("NoDgData5"),m_BtnHorde);
}

void UMapSelectPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//
	m_BtnMainDG->OnClicked.AddDynamic(this,&UMapSelectPanel::ShowMainDG);
	m_BtnRiteDG->OnClicked.AddDynamic(this,&UMapSelectPanel::ShowRiteDG);
	m_BtnEventDG->OnClicked.AddDynamic(this,&UMapSelectPanel::ShowEventDG);
	//
	m_BtnClose->OnClicked.AddDynamic(this,&UMapSelectPanel::CloseDgPanel);
}

void UMapSelectPanel::InitDgButton(const FDungeonDataRow* dgData, UMapSelectButton* SlotCreated)
{
	SlotCreated->InitButton(dgData);
	
	SlotCreated->m_OnDgClicked.AddUObject(this,&UMapSelectPanel::OpenMapInfoPopup);
}

UMapSelectButton* UMapSelectPanel::CreateDgBtn(const FDungeonDataRow* dgData)
{
	UMapSelectButton* SlotCreated = CreateWidget<UMapSelectButton>(this, m_ClassMapSelectBtn);

	InitDgButton(dgData, SlotCreated);
	
	return SlotCreated;
}

void UMapSelectPanel::OpenMapInfoPopup(const FDungeonDataRow* dgData)
{
	PRINTF("OpenMapPopup");

	m_MapInfoPopup->OpenPopup(dgData);
}

void UMapSelectPanel::ShowMainDG()
{
	m_PanelSwitcher->SetActiveWidget(m_MainDungeonPanel);
}

void UMapSelectPanel::ShowRiteDG()
{
	m_PanelSwitcher->SetActiveWidget(m_RiteDungeonPanel);
}

void UMapSelectPanel::ShowEventDG()
{
	m_PanelSwitcher->SetActiveWidget(m_EventDungeonPanel);
}


void UMapSelectPanel::PowerDungeon()
{
	
}

void UMapSelectPanel::KeyTypeDungeon()
{
	
}

void UMapSelectPanel::EventGoldDungeon()
{
	
}

void UMapSelectPanel::EventCowRoom()
{
	
}


void UMapSelectPanel::CloseDgPanel()
{
	TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
	
	DiaPC.Get()->CloseMapSelectMenu();
	
	ShowMainDG();
}
