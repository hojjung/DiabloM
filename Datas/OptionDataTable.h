// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "GenericPlatformMisc.h"
#include "ObjectMacros.h"

#include "OptionDataTable.generated.h"


//옵션부터 다시 만들어야할듯
//옵션의 상수 데이터
//랜덤 최소 최대치
//옵션 게임플레이 태그
//옵션 게임 이펙트
struct FOption;
USTRUCT(BlueprintType) //난이도,티어
struct FOptionSpec
{
    GENERATED_BODY()
    //It is saving
    
public:
    FOptionSpec(): m_fValue(0), m_DataOption(nullptr)
    {
    }

    UPROPERTY(EditAnywhere)
    float m_fValue;
    UPROPERTY(EditAnywhere)
    FName m_OptionID;

    const FOption* m_DataOption;
    
    FText GetOptionText() const;
};

USTRUCT(BlueprintType) //난이도,티어
struct FOption : public FTableRowBase
{
    GENERATED_BODY()

public:
    FOption()
    {
        m_fMinValue.Init(1,3);
        m_fMinValue[1] =3;
        m_fMinValue[2] =5;
        
        m_fMaxValue.Init(10,3);
        m_fMaxValue[1] =25;
        m_fMaxValue[2] =45;
        
        m_FormatArguSet = "{0}{1}{2}";//need open?
        m_FormatEffect = FText::FromString("Ex)% Attack Bonus");
        m_OptionTag = FGameplayTag::RequestGameplayTag("Item.Option",true);
        m_OptionID="SetSameWithRowName";
        
        m_bIsPercentValue=false;

        static ConstructorHelpers::FObjectFinder<UTexture> FoundTexture(
        TEXT("Texture2D'/Game/Sprite/UI/fg4_iconsSilver_marker.fg4_iconsSilver_marker'"));
        m_OptionIcon=FoundTexture.Object;

        //
    }
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UTexture* m_OptionIcon;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName m_OptionID;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText m_FormatEffect;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool m_bIsPercentValue;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<float> m_fMinValue;//35 -> 35% -> 1.35
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<float> m_fMaxValue;
    //UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FString m_FormatArguSet;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag m_OptionTag;
    
private:
    FText Plus = FText::FromString("+");
    FText Minus = FText::FromString("-");
    //char Percent = '%';
    //char NoPercent = ' ';
    
public:
    FORCEINLINE FText GetOptionFormat(float optionValue) const
    {
        FFormatOrderedArguments Args;

        if(optionValue ==0)
        {
            PRINTF("WTF - Value Is Zero");
            Args.Add(Plus);
        }
        else if(optionValue<0)
        {
            Args.Add(Minus);
        }
        else
        {
            Args.Add(Plus);
        }
        
        if(m_bIsPercentValue)
        {
            optionValue-=1.f;
            optionValue*=100.f;
            //1.35 -> 0.35 -> 35%
        }

        Args.Add(UKismetTextLibrary::Conv_FloatToText(optionValue,ERoundingMode::FromZero,
            false,true,1,324,1,1));

        Args.Add(m_FormatEffect);
        
        FTextFormat FormatT = FText::FromString(m_FormatArguSet);

        return FText::Format(FormatT, Args);
    }

    FORCEINLINE FOptionSpec MakeOptionInst() const
    {
        FOptionSpec NewOption;

        NewOption.m_fValue = FMath::RandRange(m_fMinValue.GetRandom(),m_fMaxValue.GetRandom());

        NewOption.m_OptionID=m_OptionID;

        NewOption.m_DataOption=this;

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

    static FText GetOptionText(const FOptionSpec& spec);
};


//옵션의 포맷 효과 텍스트
//옵션 값의 최소 최대치
//옵션의 실제 이펙트
