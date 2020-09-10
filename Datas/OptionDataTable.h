// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "OptionDataTable.generated.h"


USTRUCT(BlueprintType) //난이도,티어
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


    FText GetOptionFormat(int value) const
    {
        FFormatOrderedArguments Args;
        Args.Add(value);
        Args.Add(m_FormatEffect);

        FTextFormat FormatT = FText::FromString(m_FormatArguSet);

        return FText::Format(FormatT, Args);
    }
};

USTRUCT(BlueprintType) //난이도,티어
struct FOptionInstance
{
    GENERATED_BODY()

public:
    FOptionInstance(): m_fValue(0)
    {
        m_OptionData = nullptr;
    }

    const FOption* m_OptionData; //i should change this for save
    float m_fValue;
};

USTRUCT(BlueprintType)
struct FOptionValue
{
    GENERATED_BODY()

public:
    FOptionValue()
    {
        m_nIndex = -1;
        m_fValue = 0;
    }

    FOptionValue(int index, float v)
    {
        m_nIndex = index;
        m_fValue = v;
    }

    UPROPERTY(EditAnywhere)
    int m_nIndex;
    UPROPERTY(EditAnywhere)
    float m_fValue;
};

UCLASS()
class DIABLOM_API UOptionDataTable : public UObject
{
    GENERATED_BODY()

public:
    UOptionDataTable();
public:
    static UDataTable* GetOptionTable;

public:
    static const FOption& GetOption(FName id);

    static const FOption* GetOptionPtr(FName id);
};


//옵션의 포맷 효과 텍스트
//옵션 값의 최소 최대치
//옵션의 실제 이펙트
