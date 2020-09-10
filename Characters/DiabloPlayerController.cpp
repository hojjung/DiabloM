#include "DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloCheatManager.h"
#include "Datas/ItemDataTable.h"
#include "Item/Inventory.h"
#include "Item/EquipmentSystem.h"
#include "Item/DroppedItem.h"

 
ADiabloPlayerController*  ADiabloPlayerController::Get=nullptr;

ADiabloPlayerController::ADiabloPlayerController()
{
	ADiabloPlayerController::Get=this;
	
	CheatClass = UDiabloCheatManager::StaticClass();
	m_ClassMainMenu = UMainCanvas::StaticClass();

	m_nInvenX = 5;
	m_nInvenY = 8;

	bShowMouseCursor=true;

	APlayerController::SetVirtualJoystickVisibility(true);
}

void ADiabloPlayerController::BeginPlay()
{
	Super::BeginPlay();

	m_Inven = NewObject<UInventory>();
	m_Inven->InitInven(m_nInvenX,m_nInvenY);

	m_EquipSystem = NewObject<UEquipmentSystem>();
	m_EquipSystem->Init();

	InitWidget();

	
}

void ADiabloPlayerController::InitWidget()
{
	m_MainMenu = CreateWidget<UMainCanvas>(this, m_ClassMainMenu, "MainMenu00");
	m_MainMenu->AddToViewport();
	m_MainMenu->Init(this,Cast<APlayerDiabloCharacter>(GetPawn()),m_EquipSystem,m_Inven);

	CloseMainMenu();
}

void ADiabloPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}

void ADiabloPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Exit", EInputEvent::IE_Pressed, this, &ADiabloPlayerController::ExitGame);
	InputComponent->BindAction("OpenMainMenu", EInputEvent::IE_Pressed, this, &ADiabloPlayerController::OpenMainMenu);
}

void ADiabloPlayerController::ExitGame()
{
	PRINTF("Exit");
	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, true);
}

void ADiabloPlayerController::MonsterTargetIn(AUnitPawn * targetPawn)
{
}

void ADiabloPlayerController::MonsterTargetOut(AUnitPawn * targetPawn)
{
}

void ADiabloPlayerController::PrintStat()
{
	Cast<APlayerDiabloCharacter>(GetPawn())->PrintStats();
}

void ADiabloPlayerController::PrintInven()
{
	m_Inven->PrintInven();
}

void ADiabloPlayerController::PrintEquipment()
{
	m_EquipSystem->PrintEquipStats();
}

bool ADiabloPlayerController::AddItem(FItemInstance itemInst)
{
	return m_Inven->AddItemAuto(itemInst);
}

bool ADiabloPlayerController::PickUpItem(ADroppedItem * pickupItem)
{
	return AddItem(pickupItem->GetItemInstance());
	//return true;
}

void ADiabloPlayerController::OpenMainMenu()
{
	if (m_MainMenu->GetVisibility() != ESlateVisibility::Hidden)
	{
		CloseMainMenu();
		return;
	}

	//UWidgetBlueprintLibrary::SetInputMode_UIOnly(this);
	m_MainMenu->OpenMainMenu();

	APlayerController::SetVirtualJoystickVisibility(false);
}

void ADiabloPlayerController::CloseMainMenu()
{
	//UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
	m_MainMenu->CloseMainMenu();
	APlayerController::SetVirtualJoystickVisibility(true);
}

APlayerDiabloCharacter* ADiabloPlayerController::GetUnitPawn()
{
	return  Cast<APlayerDiabloCharacter>( GetPawn());
}
