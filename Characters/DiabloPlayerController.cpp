#include "DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloCheatManager.h"
#include "Datas/ItemDataTable.h"
#include "Objs/Inventory.h"
#include "Objs/PlayerSkillSystem.h"
#include "Objs/EquipmentSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Objs/DroppedItem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


ADiabloPlayerController*  ADiabloPlayerController::Get=nullptr;

ADiabloPlayerController::ADiabloPlayerController()
{
	ADiabloPlayerController::Get=this;
	
	CheatClass = UDiabloCheatManager::StaticClass();
	m_ClassMainMenu = UMainCanvas::StaticClass();

	m_nInvenX = 5;
	m_nInvenY = 8;

	bShowMouseCursor=true;
}

void ADiabloPlayerController::BeginPlay()
{
	Super::BeginPlay();

	m_Inven = new Inventory();
	m_Inven->InitInven(m_nInvenX,m_nInvenY);

	m_SkillSystem = new PlayerSkillSystem();

	m_EquipSystem = new EquipmentSystem();
	m_EquipSystem->Init();

	InitWidget();

	
}

void ADiabloPlayerController::InitWidget()
{
	m_MainMenu = CreateWidget<UMainCanvas>(this, m_ClassMainMenu, "MainMenu00");
	m_MainMenu->AddToViewport();
	m_MainMenu->Init(this,Cast<APlayerDiabloCharacter>(GetPawn()),m_SkillSystem ,m_EquipSystem,m_Inven);

	CloseMainMenu();
}

void ADiabloPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	delete m_EquipSystem;
	delete m_Inven;
	delete m_SkillSystem;
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
	static int Index = 0;
	return m_Inven->AddItem(Index++,itemInst);
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
}

void ADiabloPlayerController::CloseMainMenu()
{
	//UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
	m_MainMenu->CloseMainMenu();
}
