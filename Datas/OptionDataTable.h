// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "AbilitySystem/AbilityTypes.h"
#include "ConstructorHelpers.h"
#include "OptionDataTable.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UOptionDataTable : public UObject
{
	GENERATED_BODY()
	
};

USTRUCT(BlueprintType)//난이도,티어
struct FOption : public FTableRowBase
{
	GENERATED_BODY()

public:
	FOption()
	{
		m_bIsPercent = false;
		m_fMinValue = 10.f;
		m_fMaxValue = 40.f;
		m_FormatArguSet = "Ex)+ {0} {1}";
		m_FormatEffect = FText::FromString("Ex)Increase Attack");
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool m_bIsPercent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fMinValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fMaxValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString m_FormatArguSet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_FormatEffect;



	FText GetOptionFormat(float value) const
	{

		FFormatOrderedArguments Args;
		Args.Add(value);
		Args.Add(m_FormatEffect);

		FTextFormat FormatT = FText::FromString(m_FormatArguSet);

		return FText::Format(FormatT, Args);
	}
};


USTRUCT(BlueprintType)//난이도,티어
struct FOptionInstance
{
	GENERATED_BODY()

public:
	FOptionInstance()
	{
		m_OptionData = nullptr;
	}

	const FOption* m_OptionData;//i should change this for save
	float m_fValue;

};




//옵션의 포맷 효과 텍스트
//옵션 값의 최소 최대치
//옵션의 실제 이펙트