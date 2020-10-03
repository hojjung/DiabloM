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

	check( m_ClassDmgText);
	
	m_Inven = NewObject<UInventory>();
	m_Inven->InitInven(m_nInvenX,m_nInvenY);

	m_EquipSystem = NewObject<UEquipmentSystem>();
	m_EquipSystem->Init(GetPlayerPawn()->GetDiaAbilitySystem());
	m_EquipSystem->GetItemChangeCallback().AddUObject(this,&ADiabloPlayerController::PlayerMeshChange);
	InitWidget();

	USaveLoadManager::Get->CreateSetPlayerCharacter();
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

void ADiabloPlayerController::PlayerMeshChange(int slot, FItemInstance& item)
{
	GetPlayerPawn()->EquipMesh(&item,static_cast<ESlotsEquipAry>(slot));
}

APlayerDiabloCharacter* ADiabloPlayerController::GetPlayerPawn()
{
	return  Cast<APlayerDiabloCharacter>( GetPawn());
}

void ADiabloPlayerController::ShowDamageNumber(const float local_damage_done, AUnitPawn* unit_pawn) //target
{
	//need object pool
	UDamageTextWidgetComponent* DamageText = NewObject<UDamageTextWidgetComponent>(unit_pawn, m_ClassDmgText);
	DamageText->RegisterComponent();
	DamageText->AttachToComponent(unit_pawn->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	DamageText->SetDamageText(local_damage_done);

}

void ADiabloPlayerController::HideFocusStatusWidget()
{
	m_MainMenu->HideMonsterInfo();
}

void ADiabloPlayerController::ShowFocusStatusWidget(AUnitPawn* unit)
{
	m_MainMenu->ShowMonsterInfo(unit);
}
