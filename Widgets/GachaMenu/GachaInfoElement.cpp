#include "GachaInfoElement.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"



void UGachaInfoElement::SetInfoText(const FGachaTableRow* gachaTable,float totalPercent)
{
	m_TextGachaName->SetText(gachaTable->GetGachaData().m_ShowingName);

	m_TextGachaName->SetColorAndOpacity(gachaTable->GetGachaData().GetTier()->m_TierColor);

	float Percent = gachaTable->GetPercent(totalPercent);

	FFormatOrderedArguments Args;

	Args.Add(FText::FromString(FString::Printf(TEXT("%.5f"),Percent*100.f)));
	// FTextFormat
	 FTextFormat Format = FText::FromString("{0}%");

	m_TextGachaPercent->SetText(FText::Format(Format,Args));
}
