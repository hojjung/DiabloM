// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineMinimal.h"
#include "Engine/Engine.h"

DIABLOM_API DECLARE_LOG_CATEGORY_EXTERN(DiaM, Log, All);


#define PRINTFSCREEN(Format, ...) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, *FString::Printf(TEXT(Format), ##__VA_ARGS__),true)
#define PRINTF(Format, ...) UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT(Format), ##__VA_ARGS__)) PRINTFSCREEN(Format, ##__VA_ARGS__)

#define TEST_BIT(Bitmask, Bit) (((Bitmask) & (static_cast<uint32>(Bit)))>0)
#define SET_BIT(Bitmask, Bit) (Bitmask |= static_cast<uint32>(Bit))
#define CLEAR_BIT(Bitmask, Bit) (Bitmask &= ~( static_cast<uint32>(Bit)))
#define TOFLAG(Enum) ( static_cast<uint32>(Enum))
