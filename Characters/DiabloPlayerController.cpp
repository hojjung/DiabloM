#include "DiabloPlayerController.h"
#include "OnlineSubsystem.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloCheatManager.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"

ADiabloPlayerController::ADiabloPlayerController()
{
	
	CheatClass = UDiabloCheatManager::StaticClass();
	m_DmgIndex=0;
	bShowMouseCursor=true;

	APlayerController::SetVirtualJoystickVisibility(true);

	m_FormatMiss=FTextFormat::FromString("Miss-{0}%");

	
	//
	//
	////WidgetBlueprint'/Game/Blueprints/Widgets/MainMenus/WB_GameOver.WB_GameOver'
	//Blueprint'/Game/Blueprints/Widgets/WorldWidget/WC_DamageText.WC_DamageText'
}

void ADiabloPlayerController::InitWidget()
{
	APlayerDiabloCharacter* PlayerPawn=Cast<APlayerDiabloCharacter>(GetPawn());
	
	CreateDmgWC(25);
}

void ADiabloPlayerController::CreateDmgWC(int count)
{
	m_AryDmgWC.Reset();
	for(int i=0; i<count;i++)
	{
		UDamageTextWidgetComponent* DamageText = NewObject<UDamageTextWidgetComponent>(GetPawn(), UDamageTextWidgetComponent::StaticClass());
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

void ADiabloPlayerController::OnDeviceBackKey()
{
	ExitGame();
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

void ADiabloPlayerController::BackToSelectMenu()
{
	PRINTF("Continue StartMenu");
	ClientForceGarbageCollection();
	UGameplayStatics::OpenLevel(GetWorld(),"StartMenu");
}

