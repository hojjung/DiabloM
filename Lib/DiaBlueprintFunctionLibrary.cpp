// Fill out your copyright notice in the Description page of Project Settings.


#include "DiaBlueprintFunctionLibrary.h"


const  FText UDiaBlueprintFunctionLibrary::UnitSymbol[9] = {
    FText::FromString(""),
    FText::FromString("K"),
    FText::FromString("M"),
    FText::FromString("G"),
    FText::FromString("T"),
    FText::FromString("P"),
    FText::FromString("E"),
    FText::FromString("Z"),
    FText::FromString("Y")
};

const FTextFormat UDiaBlueprintFunctionLibrary::FormatT= FTextFormat(FText::FromString("{0}{1}"));
