#include "DiabloPlayerController.h"

#include "MonsterPawn.h"
#include "OnlineSubsystem.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Kismet/KismetInputLibrary.h"
#include "Managers/DiabloCheatManager.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Managers/DiabloGameInstance.h"

ADiabloPlayerController::ADiabloPlayerController()
{
	CheatClass = UDiabloCheatManager::StaticClass();
	m_DmgIndex=0;
	bShowMouseCursor=true;

	APlayerController::SetVirtualJoystickVisibility(true);

	m_FormatMiss=FTextFormat::FromString("Miss-{0}%");

	static ConstructorHelpers::FClassFinder<UDamageTextWidgetComponent> FoundW(
   TEXT("Blueprint'/Game/Blueprints/Widget/WorldWidget/WC_DamageText.WC_DamageText_C'"));
	m_ClassDW=FoundW.Class;
	//Blueprint'/Game/Blueprints/NewWidget/WC_DamageText.WC_DamageText'
	////WidgetBlueprint'/Game/Blueprints/Widgets/MainMenus/WB_GameOver.WB_GameOver'
	//Blueprint'/Game/Blueprints/Widgets/WorldWidget/WC_DamageText.WC_DamageText'
}

void ADiabloPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitWidget();

	m_MovePointIndicator  = GetWorld()->SpawnActor<AMovePointIndicator>(AMovePointIndicator::StaticClass());
	HideMoveIndicator();


	
}

void ADiabloPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	m_AryIgnoreActors.Add(this);
	m_AryIgnoreActors.Add(InPawn);

	m_AryQuery.Add(EObjectTypeQuery::ObjectTypeQuery1);
	m_AryQuery.Add(EObjectTypeQuery::ObjectTypeQuery2);
	m_AryQuery.Add(EObjectTypeQuery::ObjectTypeQuery3);
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
		UDamageTextWidgetComponent* DamageText = NewObject<UDamageTextWidgetComponent>(GetPawn(), m_ClassDW);
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

void ADiabloPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Exit", EInputEvent::IE_Pressed, this, &ADiabloPlayerController::ExitGame);
	InputComponent->BindAction("AndroidBack", EInputEvent::IE_Pressed, this, &ADiabloPlayerController::OnDeviceBackKey);
	InputComponent->BindAction("MouseClick", EInputEvent::IE_Pressed, this, &ADiabloPlayerController::ClickActor);
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

void ADiabloPlayerController::ShowDamageNumber(const BigInt& local_damage_done,AUnitPawn* unit_pawn,EDamagePopup dmgPopup) //target
{
	UDamageTextWidgetComponent* DamageText = GetDmgWC();
	
	DamageText->SetWorldLocation(unit_pawn->GetActorLocation());
	
	if(dmgPopup==EDamagePopup::Miss)
	{
		FFormatOrderedArguments Args;
		Args.Add(local_damage_done.ToInt());
		DamageText->SetDamageText(FText::Format(m_FormatMiss,Args));//
	}
	else
	{
		DamageText->SetDamageText(FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(local_damage_done)));//
	}
	
	DamageText->StartAnimation(dmgPopup);
}

void ADiabloPlayerController::ShowDamageText(const FString& stringWant, AUnitPawn* unit_pawn, EDamagePopup dmgPopup)
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


void ADiabloPlayerController::ClickActor()
{
	APlayerDiabloCharacter* DiaPlayer = Cast<APlayerDiabloCharacter>( GetPawn());

	FVector StartPos;
	
	FVector EndPos;

	DeprojectMousePositionToWorld(StartPos,EndPos);

	EndPos*=10000.f;

	EndPos+=StartPos;

	FNavLocation Loc;

	FHitResult Hits;
	//StartPos = DiaPlayer->GetCameraLoc();

	if(!UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		StartPos,EndPos,
		m_AryQuery,
		false,m_AryIgnoreActors,EDrawDebugTrace::ForOneFrame,Hits,true))
	{
		return;
	}
	PRINTF("ClickSuccess");



	AMonsterPawn* Mob = Cast<AMonsterPawn>( Hits.Actor);

	
	if(!Mob)
	{
		if(!UDiabloGameInstance::Get->GetNavSys()->ProjectPointToNavigation(Hits.Location,Loc))
		{
			PRINTF("Not ReachAble");
			return;
		}
		DiaPlayer->FocusTarget(nullptr);
		DiaPlayer->SetManualMoveLocation(Hits.Location);
		ShowMoveIndicator(Hits.Location);
		return;
	}
	HideMoveIndicator();
	
	DiaPlayer->FocusTarget(Mob);
}

void ADiabloPlayerController::ShowMoveIndicator(FVector loc)
{
	m_MovePointIndicator->SetActorHiddenInGame(false);
	m_MovePointIndicator->SetActorLocation(loc);
}

void ADiabloPlayerController::HideMoveIndicator()
{
	m_MovePointIndicator->SetActorHiddenInGame(true);
}

void ADiabloPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	m_OnTick.Broadcast(DeltaSeconds);
}

