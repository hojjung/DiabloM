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


ADiabloPlayerController::ADiabloPlayerController()
{
	CheatClass = UDiabloCheatManager::StaticClass();
	m_ClassMainMenu = UDefaultMenu::StaticClass();
	m_nInvenX = 5;
	m_nInvenY = 8;
}

void ADiabloPlayerController::BeginPlay()
{
	Super::BeginPlay();
	m_Inven = new Inventory();
	m_Inven->InitInven(m_nInvenX,m_nInvenY);

	m_SkillSystem = new PlayerSkillSystem();

	m_EquipSystem = new EquipmentSystem();


	InitWidget();
}

void ADiabloPlayerController::InitWidget()
{
	m_MainMenu = CreateWidget<UDefaultMenu>(this, m_ClassMainMenu, "MainMenu00");
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

bool ADiabloPlayerController::AddItem(FItemInstance itemInst)
{
	return m_Inven->AddItemAuto(itemInst);
}

bool ADiabloPlayerController::PickUpItem(ADroppedItem * pickupItem)
{
	return AddItem(pickupItem->GetItemInstance());
}

void ADiabloPlayerController::OpenMainMenu()
{
	if (m_MainMenu->GetVisibility() != ESlateVisibility::Hidden)
	{
		CloseMainMenu();
		return;
	}

	//UWidgetBlueprintLibrary::SetInputMode_UIOnly(this);
	bShowMouseCursor = true;
	m_MainMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void ADiabloPlayerController::CloseMainMenu()
{
	//UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
	m_MainMenu->SetVisibility(ESlateVisibility::Hidden);
	bShowMouseCursor = false;
}
