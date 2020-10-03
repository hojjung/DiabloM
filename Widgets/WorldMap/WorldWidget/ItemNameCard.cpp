
#include "ItemNameCard.h"

void UItemNameCard::SetItemName(const FText & itemName)
{
	m_ItemName->SetText(itemName);
}

void UItemNameCard::SetItemColor(const FLinearColor & colorWant)
{
	m_ItemName->SetColorAndOpacity(FSlateColor(colorWant));
}
