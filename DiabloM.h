// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ConstructorHelpers.h"
//
//
#include "Engine/Engine.h"
#include "EngineMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
//
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture.h"
#include "Engine/Font.h"
//
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "Components/SizeBox.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/Overlay.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
//
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
//
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"
//
#include "WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
//
#include "DrawDebugHelpers.h"
//
#include "BigInt.h"
#include "Components/SceneComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Net/UnrealNetwork.h"
//
// #include "AbilitySystemComponent.h"
// #include "AttributeSet.h"
// #include "GameplayEffectTypes.h"
// #include "Abilities/GameplayAbilityTargetTypes.h"
// #include "Abilities/GameplayAbility.h"
// #include "GameplayTagContainer.h"
// #include "GameplayEffectExecutionCalculation.h"
// #include "Abilities/Tasks/AbilityTask.h"
// #include "Abilities/GameplayAbilityTypes.h"
// #include "AbilitySystem/AbilityTypes.h"

//

DIABLOM_API DECLARE_LOG_CATEGORY_EXTERN(DiaM, Log, All);


#define PRINTFSCREEN(Format, ...) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, *FString::Printf(TEXT(Format), ##__VA_ARGS__),true)
#define PRINTF(Format, ...) UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT(Format), ##__VA_ARGS__)) PRINTFSCREEN(Format, ##__VA_ARGS__)

#define TEST_BIT(Bitmask, Bit) (((Bitmask) & (1 << static_cast<uint32>(Bit))) > 0)
//#define TEST_BIT(Bitmask, Bit) (((Bitmask) & (static_cast<uint32>(Bit)))>0)
#define SET_BIT(Bitmask, Bit) (Bitmask |= static_cast<uint32>(Bit))
#define CLEAR_BIT(Bitmask, Bit) (Bitmask &= ~( static_cast<uint32>(Bit)))
#define TOFLAG(Enum) ( static_cast<uint32>(Enum))

//#define GETENUMSTRING(etype, evalue) ( (FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true) != nullptr) ? FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true)->GetNameStringByValue((int64)evalue) : FString("Invalid - are you sure enum uses UENUM() macro?") )

#define MAXLEVEL 100

#define INVEN_X 6

#define INVEN_Y 7

#define STORAGE_X 6

#define STORAGE_Y 6

#define SHOP_X 6

#define SHOP_Y 6

#define SHOPRESELL_X 6

#define SHOPRESELL_Y 7

#define CURRSLOT 2

#define MAXSLOT 7

#define MAXVALUE 99999999999999999999.f 

#define stringify(name) #name
#define EnumToStr(Enum, Val)  EnumToString( (stringify(Enum)) , Val)

template<typename T>
static FString EnumToString(const FString& enumName, const T value)
{
	UEnum* pEnum = FindObject<UEnum>(ANY_PACKAGE, *enumName);
	return *(pEnum ? pEnum->GetNameStringByIndex(static_cast<uint8>(value)) : "NoEnum-Null");
}

typedef  TBigInt<512,true> BigInt;
//#define LOCTEXT_NAMESPACE "FMyEditorExtensionModule"
//#undef LOCTEXT_NAMESPACE

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTick,float);