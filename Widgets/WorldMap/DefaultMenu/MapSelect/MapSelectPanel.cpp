#include "MapSelectPanel.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DungeonManager.h"

UMapSelectPanel::UMapSelectPanel(const FObjectInitializer& objInit):Super(objInit)
{
	
}

void UMapSelectPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnDefaultInf->OnClicked.AddDynamic(this,&UMapSelectPanel::EnterDefaultInfDungeon);
	m_BtnDemon->OnClicked.AddDynamic(this,&UMapSelectPanel::EnterDemonTypeDungeon);
	m_BtnBeast->OnClicked.AddDynamic(this,&UMapSelectPanel::EnterBeastTypeDungeon);
	m_BtnUndead->OnClicked.AddDynamic(this,&UMapSelectPanel::EnterUndeadTypeDungeon);
	m_BtnHorde->OnClicked.AddDynamic(this,&UMapSelectPanel::EnterHordeTypeDungeon);
	//
	m_BtnMainDG->OnClicked.AddDynamic(this,&UMapSelectPanel::ShowMainDG);
	m_BtnRiteDG->OnClicked.AddDynamic(this,&UMapSelectPanel::ShowRiteDG);
	m_BtnEventDG->OnClicked.AddDynamic(this,&UMapSelectPanel::ShowEventDG);
	
}

FReply UMapSelectPanel::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Result = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return Result;
}

UMapSelectButton* UMapSelectPanel::CreateDgBtn(const FDungeonDataRow* dgData)
{
	UMapSelectButton* SlotCreated = CreateWidget<UMapSelectButton>(this, m_ClassMapSelectBtn);

	SlotCreated->InitButton(dgData);
	
	SlotCreated->m_OnDgClicked.AddUObject(this,&UMapSelectPanel::OpenMapInfoPopup);
	
	return SlotCreated;
}

void UMapSelectPanel::OpenMapInfoPopup(const FDungeonDataRow* dgData)
{

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

void UMapSelectPanel::EnterDefaultInfDungeon()
{
	UDiabloGameInstance::Get->GetDungeonManager()->CreateDefaultInfinityDungeon(1);
}

void UMapSelectPanel::EnterDemonTypeDungeon()
{
	
}

void UMapSelectPanel::EnterBeastTypeDungeon()
{
	
}

void UMapSelectPanel::EnterUndeadTypeDungeon()
{
	
}

void UMapSelectPanel::EnterHordeTypeDungeon()
{
	
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
