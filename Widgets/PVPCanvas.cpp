#include "PVPCanvas.h"
#include "CommonElement/MaterialProgressBar.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Managers/DiabloGameInstance.h"

void UPVPCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UDiabloGameInstance::Get->m_PVPManager->m_OnDmgChanged.AddUObject(this,&UPVPCanvas::UpdateDmgGauge);
}

void UPVPCanvas::UpdateDmgGauge(float percentOne, BigInt playerDmg, BigInt otherPlayerDmg)
{
	m_BarPlayerDmg->SetProgressValue(percentOne);
	
	m_TxtPlayerDmgValue->SetText(FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(playerDmg)));
	m_TxtOtherPlayerDmgValue->SetText(FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(otherPlayerDmg)));
}

void UPVPCanvas::ReqeustPopupText(const FText& str)
{
	m_TxtRequestedInfo->SetText(str);
}
