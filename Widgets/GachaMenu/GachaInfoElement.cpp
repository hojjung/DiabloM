#include "GachaInfoElement.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"



void UGachaInfoElement::SetInfoText(const FGachaTableRow* gachaTable,float totalPercent)
{
	m_TextGachaName->SetText(gachaTable->GetGachaData().m_ShowingName);

	float Percent = gachaTable->GetPercent(totalPercent);

	FFormatOrderedArguments Args;

	Args.Add(FText::FromString(FString::Printf(TEXT("%.5f"),Percent)));
	// FTextFormat
	 FTextFormat Format = FText::FromString("{0}%");

	m_TextGachaPercent->SetText(FText::Format(Format,Args));
}
