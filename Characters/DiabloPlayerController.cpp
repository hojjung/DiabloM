#include "DiabloPlayerController.h"

#include "OnlineSubsystem.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloCheatManager.h"
#include "Datas/ItemDataTable.h"
#include "Item/Inventory.h"
#include "Item/EquipmentSystem.h"
#include "Item/DroppedItem.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Managers/StartMap/PlayerCreateManager.h"


ADiabloPlayerController*  ADiabloPlayerController::Get=nullptr;

ADiabloPlayerController::ADiabloPlayerController()
{
	ADiabloPlayerController::Get=this;
	
	CheatClass = UDiabloCheatManager::StaticClass();
	m_ClassMainMenu = UMainCanvas::StaticClass();

	m_DmgIndex=0;
	bShowMouseCursor=true;

	APlayerController::SetVirtualJoystickVisibility(true);

	m_FormatMiss=FTextFormat::FromString("Miss-{0}%");
}

void ADiabloPlayerController::InitPlCtrlAndWidget()
{
	check(m_ClassDmgText);
	GetPlayerPawn()->Init();
	m_Inven = NewObject<UInventory>();
	m_Inven->InitInven(INVEN_X,INVEN_Y);
	m_EquipSystem = NewObject<UEquipmentSystem>();
	m_EquipSystem->Init(GetPlayerPawn()->GetDiaAbilitySystem());
	
	m_AryStorage.Reset();
	int i=0;
	while (i++<5)
	{
		UInventory* Storage = NewObject<UInventory>();
		Storage->InitInven(STORAGE_X,STORAGE_Y);
		m_AryStorage.Emplace(Storage);
	}
	m_AryStorageOpend.Init(false,5);
	m_AryStorageOpend[0]=true;
	
	USaveLoadManager::Get->CreateSetPlayerCharacter();//Set Every SaveFile to Load
	InitWidget();
}


void ADiabloPlayerController::InitWidget()
{
	APlayerDiabloCharacter* PlayerPawn=Cast<APlayerDiabloCharacter>(GetPawn());
	m_MainMenu = CreateWidget<UMainCanvas>(this, m_ClassMainMenu, "MainMenu00");
	m_MainMenu->AddToViewport();
	m_MainMenu->Init(this,PlayerPawn,m_EquipSystem,m_Inven,&m_AryStorage);
	m_MainMenu->CloseMainMenu();
	
	m_GameOverScreen = CreateWidget<UDiaGameOverScreen>(this, m_ClassGameOver, "GameOverScreen00");
	m_GameOverScreen->AddToViewport();
	m_GameOverScreen->Init(this,PlayerPawn);
	m_GameOverScreen->SetVisibility(ESlateVisibility::Hidden);
	PlayerPawn->GetOnDied().AddUObject(this,&ADiabloPlayerController::OnPlayerDied);
	PlayerPawn->GetOnRevived().AddUObject(this,&ADiabloPlayerController::OnPlayerRevived);
	
	CreateDmgWC(15);
}

void ADiabloPlayerController::CreateDmgWC(int count)
{
	m_AryDmgWC.Reset();
	for(int i=0; i<count;i++)
	{
		UDamageTextWidgetComponent* DamageText = NewObject<UDamageTextWidgetComponent>(GetPlayerPawn(), m_ClassDmgText);
		DamageText->RegisterComponent();
		m_AryDmgWC.Add(DamageText);
		DamageText->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->SetHiddenInGame(true);
	}
}

UDamageTextWidgetComponent* ADiabloPlayerController::GetDmgWC()
{
	auto* Dmg=m_AryDmgWC[m_DmgIndex++];

	if(m_DmgIndex>=m_AryDmgWC.Num())
	{
		m_DmgIndex=0;
	}
	
	Dmg->SetHiddenInGame(false);
	
	return Dmg;
}

void ADiabloPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ADiabloPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Exit", EInputEvent::IE_Pressed, this, &ADiabloPlayerController::ExitGame);
	InputComponent->BindAction("AndroidBack", EInputEvent::IE_Pressed, this, &ADiabloPlayerController::OnDeviceBackKey);
}

void ADiabloPlayerController::ExitGame()
{
	PRINTF("Exit");
	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, true);

}

void ADiabloPlayerController::OnPlayerDied(AUnitPawn* player)
{
	m_MainMenu->SetVisibility(ESlateVisibility::Hidden);
	m_GameOverScreen->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	APlayerController::SetVirtualJoystickVisibility(false);
}

void ADiabloPlayerController::OnPlayerRevived(AUnitPawn* player)
{
	m_MainMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	m_GameOverScreen->SetVisibility(ESlateVisibility::Hidden);
	APlayerController::SetVirtualJoystickVisibility(true);
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
	return AddItem(pickupItem->GetCurrentItem());
	//return true;
}

void ADiabloPlayerController::OnDeviceBackKey()
{
	m_MainMenu->CloseMainMenu();
	
	ExitGame();
}


void ADiabloPlayerController::OnWidgetOpenClose(bool isOpen)
{
	UGameplayStatics::SetGamePaused(this->GetWorld(),isOpen);
	this->SetVirtualJoystickVisibility(!isOpen);
}

APlayerDiabloCharacter* ADiabloPlayerController::GetPlayerPawn()
{
	return  Cast<APlayerDiabloCharacter>( GetPawn());
}

void ADiabloPlayerController::ShowDamageNumber(const float local_damage_done,AUnitPawn* unit_pawn,EDamagePopup dmgPopup) //target
{
	UDamageTextWidgetComponent* DamageText = GetDmgWC();
	
	DamageText->SetWorldLocation(unit_pawn->GetActorLocation());
	
	if(dmgPopup==EDamagePopup::Miss)
	{
		FFormatOrderedArguments Args;
		Args.Add(local_damage_done);
		DamageText->SetDamageText(FText::Format(m_FormatMiss,Args));//
	}
	else
	{
		DamageText->SetDamageText(UDiaBlueprintFunctionLibrary::GetAlphabetText(local_damage_done));//
	}
	
	DamageText->StartAnimation(dmgPopup);
}

void ADiabloPlayerController::ShowDamageText(const FString stringWant, AUnitPawn* unit_pawn, EDamagePopup dmgPopup)
{
	UDamageTextWidgetComponent* DamageText = GetDmgWC();
	
	DamageText->SetWorldLocation(unit_pawn->GetActorLocation());
	
	DamageText->SetDamageText(FText::FromString(stringWant));//
	
	DamageText->StartAnimation(dmgPopup);
}

UMainCanvas* ADiabloPlayerController::GetMainCanvas()
{
	return m_MainMenu;
}

void ADiabloPlayerController::BackToSelectMenu()
{
	PRINTF("Continue StartMenu");
	ClientForceGarbageCollection();
	UGameplayStatics::OpenLevel(GetWorld(),"StartMenu");
}

