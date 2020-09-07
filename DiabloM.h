// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineMinimal.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

DIABLOM_API DECLARE_LOG_CATEGORY_EXTERN(DiaM, Log, All);


#define PRINTFSCREEN(Format, ...) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, *FString::Printf(TEXT(Format), ##__VA_ARGS__),true)
#define PRINTF(Format, ...) UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT(Format), ##__VA_ARGS__)) PRINTFSCREEN(Format, ##__VA_ARGS__)

#define TEST_BIT(Bitmask, Bit) (((Bitmask) & (static_cast<uint32>(Bit)))>0)
#define SET_BIT(Bitmask, Bit) (Bitmask |= static_cast<uint32>(Bit))
#define CLEAR_BIT(Bitmask, Bit) (Bitmask &= ~( static_cast<uint32>(Bit)))
#define TOFLAG(Enum) ( static_cast<uint32>(Enum))

//#define GETENUMSTRING(etype, evalue) ( (FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true) != nullptr) ? FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true)->GetNameStringByValue((int64)evalue) : FString("Invalid - are you sure enum uses UENUM() macro?") )
#define stringify(name) #name
#define EnumToStr(Enum, Val)  EnumToString( (stringify(Enum)) , Val)

template<typename T>
static FString EnumToString(const FString& enumName, const T value)
{
	UEnum* pEnum = FindObject<UEnum>(ANY_PACKAGE, *enumName);
	return *(pEnum ? pEnum->GetNameStringByIndex(static_cast<uint8>(value)) : "NoEnum-Null");
}

UENUM(BlueprintType)
enum class ESaveVersion :uint8 //������ �ν��Ͻ��� ��ġ
{
	Init
};
