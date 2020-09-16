// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "ObjectMacros.h"

#include "OptionDataTable.generated.h"


//옵션부터 다시 만들어야할듯
//옵션의 상수 데이터
//랜덤 최소 최대치
//옵션 게임플레이 태그
//옵션 게임 이펙트

USTRUCT(BlueprintType) //난이도,티어
struct FOptionSpec
{
    GENERATED_BODY()
    //It is saving
    
public:
    FOptionSpec(): m_fValue(0),m_nOptionIndex(0)
    {
        
    }
    UPROPERTY(EditAnywhere)
    int m_fValue;
    UPROPERTY(EditAnywhere)
    int m_nOptionIndex;
};

USTRUCT(BlueprintType) //난이도,티어
struct FOption : public FTableRowBase
{
    GENERATED_BODY()

public:
    FOption()
    {
        m_bIsPercent = false;
        
        m_fMinValue.Init(1,3);
        m_fMinValue[1] =3;
        m_fMinValue[2] =5;
        
        m_fMaxValue.Init(10,3);
        m_fMaxValue[1] =25;
        m_fMaxValue[2] =45;
        
        m_FormatArguSet = "{0}{1}{2} {3}";//need open?
        m_FormatEffect = FText::FromString("Ex)Increase Attack");
        m_OptionTag = FGameplayTag::RequestGameplayTag("Item.Option",true);
    }

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool m_bIsPercent;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<int> m_fMinValue;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<int> m_fMaxValue;
    //UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FString m_FormatArguSet;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText m_FormatEffect;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag m_OptionTag;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UGameplayEffect> m_OptionGe;
    
private:
    char Plus = '+';
    char Minus = '-';
    char Percent = '%';
    char NoPercent = ' ';
    
public:
    FORCEINLINE FText GetOptionFormat(int value) const
    {
        FFormatOrderedArguments Args;

        if(value ==0)
        {
            PRINTF("WTF - Value Is Zero");
            Args.Add(Plus);
        }
        else if(value<0)
        {
            Args.Add(Minus);
        }
        else
        {
            Args.Add(Plus);
        }

        Args.Add(value);

        if(m_bIsPercent)
        {
            Args.Add(Percent);
        }
        {
            Args.Add(NoPercent);            
        }
        
        Args.Add(m_FormatEffect);
        
        FTextFormat FormatT = FText::FromString(m_FormatArguSet);

        return FText::Format(FormatT, Args);
    }

    FORCEINLINE FOptionSpec MakeOptionInst(int thisIndex) const
    {
        FOptionSpec NewOption;

        NewOption.m_fValue = FMath::RandRange(m_fMinValue.GetRandom(),m_fMaxValue.GetRandom());

        NewOption.m_nOptionIndex=thisIndex;

        return NewOption;
    }
    
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
