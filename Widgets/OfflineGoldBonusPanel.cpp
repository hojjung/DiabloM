#include "OfflineGoldBonusPanel.h"

#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Managers/DiabloGameInstance.h"


void UOfflineGoldBonusPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_FormatOfflineGold = FText::FromString("오프라인 보상으로 {0}골드를 획득했습니다.");

	m_FormatOfflineTime = FText::FromString("오프라인시간:{0}분");

	m_BtnConfirm->OnClicked.AddDynamic(this,&UOfflineGoldBonusPanel::OnConfirm);
}

void UOfflineGoldBonusPanel::SetOfflineGold(BigInt gold)
{
	FFormatOrderedArguments Args1;

	FText GoldString =FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(gold));
	
	Args1.Add(GoldString);

	m_TextOfflineGoldGained->SetText(FText::Format(m_FormatOfflineGold, Args1));

	FFormatOrderedArguments Args2;

	float Minutes = UDiabloGameInstance::Get->m_GoldManager->GetClampedOfflineMinutes();

	float PercentOne = Minutes / 1440.f;

	Args2.Add(UDiabloGameInstance::Get->m_GoldManager->GetClampedOfflineMinutes());
	
	m_TextOfflineGoldCurrent->SetText(FText::Format(m_FormatOfflineTime, Args2));

	m_OfflineTimeGauge->SetProgressValue(PercentOne);

	
}

void UOfflineGoldBonusPanel::OnConfirm()
{
	UDiabloGameInstance::Get->m_GoldManager->GainOfflineGold();

	SetVisibility(ESlateVisibility::Collapsed);
}
