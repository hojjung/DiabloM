// My First Hack n Slash


#include "ChatText.h"

void UChatText::SetNormalChat(const FString& chat)
{
	TArray<FString> OutStrAry;
	
	chat.ParseIntoArray(OutStrAry,TEXT(":"));
	
	//FString Ranking = OutStrAry[0];
	
	//FString Nickname = OutStrAry[1];
	
	//FString ChatText = OutStrAry[2];

	m_TextChat->SetText(FText::FromString(chat));
}
