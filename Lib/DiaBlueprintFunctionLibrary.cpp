#include "DiaBlueprintFunctionLibrary.h"

const  FText UDiaBlueprintFunctionLibrary::UnitSymbol[UnitSymbolMax] = {
    FText::FromString(TEXT("")),
    FText::FromString(TEXT("만")),
    FText::FromString(TEXT("억")),
    FText::FromString(TEXT("조")),
    FText::FromString(TEXT("경")),
    FText::FromString(TEXT("해")),
    FText::FromString(TEXT("자")),
    FText::FromString(TEXT("양")),
    FText::FromString(TEXT("가")),
    FText::FromString(TEXT("구")),
    FText::FromString(TEXT("간")),
    FText::FromString(TEXT("정")),
    FText::FromString(TEXT("재")),
    FText::FromString(TEXT("극")),
    FText::FromString(TEXT("항")),
    FText::FromString(TEXT("아승")),
    FText::FromString(TEXT("나유")),
    FText::FromString(TEXT("불가")),
    FText::FromString(TEXT("무량")),
    FText::FromString(TEXT("겁")),  
    FText::FromString(TEXT("구골")), 
    FText::FromString(TEXT("아산")), 
    FText::FromString(TEXT("센틸")), 
    FText::FromString(TEXT("스큐스")),
    FText::FromString(TEXT("구골플")),
    FText::FromString(TEXT("그레이")) 
};

const FTextFormat UDiaBlueprintFunctionLibrary::FormatT= FTextFormat(FText::FromString("{0}{1}"));
