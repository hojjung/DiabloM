#include "InitPlayerController.h"

#include "ConstructorHelpers.h"
#include "Managers/DiabloGameInstance.h"
#include "Widgets/InitLevelHUD.h"

AInitPlayerController::AInitPlayerController()
{
	//WidgetBlueprint'/Game/Blueprints/NewWidget/WB_NicknameSet.WB_NicknameSet'
	ConstructorHelpers::FClassFinder<USetNickname> FoundW(TEXT("WidgetBlueprint'/Game/Blueprints/NewWidget/WB_NicknameSet.WB_NicknameSet_C'"));

	bShowMouseCursor = true;
}

void AInitPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(UDiabloGameInstance::Get->m_PlayfabManager->m_bShowNicknameSet && !m_bLock)
	{
		m_bLock=true;

		Cast<AInitLevelHUD>( GetHUD())->m_Canvas->ShowNicknameSet();
	}
}

void AInitPlayerController::SetupInputComponent()
{Super::SetupInputComponent();
	InputComponent->BindAction("Exit", EInputEvent::IE_Pressed, this, &AInitPlayerController::OnDeviceBackKey);
	InputComponent->BindAction("AndroidBack", EInputEvent::IE_Pressed, this, &AInitPlayerController::OnDeviceBackKey);
}

void AInitPlayerController::OnDeviceBackKey()
{
	PRINTF("Exit");
	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, true);
}
