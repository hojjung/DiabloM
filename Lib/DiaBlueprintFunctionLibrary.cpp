#include "DiaBlueprintFunctionLibrary.h"

const  FText UDiaBlueprintFunctionLibrary::UnitSymbol[11] = {
    FText::FromString(""),
    FText::FromString("A/"),//만
    FText::FromString("B/"),//억
    FText::FromString("C/"),//조
    FText::FromString("D/"),//경
    FText::FromString("E/"),//해
    FText::FromString("F/"),//자
    FText::FromString("G/"),//양
    FText::FromString("H/"),//가
    FText::FromString("I/"),//구
    FText::FromString("J/")//간
    //정
    //재
    //극
    //항아사
    //아승기
    //나유타
    //불가사의
    //무량대수
    //겁
    //구골
    //아산키야
    //센틸리온
    //스큐스수
    //구골플렉스
    //그레이엄수
};

const FTextFormat UDiaBlueprintFunctionLibrary::FormatT= FTextFormat(FText::FromString("{0}{1}"));
