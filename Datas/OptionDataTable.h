#pragma once

#include "DiabloM.h"
#include "GameplayEffect.h"
#include "GenericPlatformMisc.h"
#include "ObjectMacros.h"
#include "AbilitySystem/GameEffect/ItemOptionGameEffect.h"

#include "OptionDataTable.generated.h"


class UPlayerDiabloAbility;
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

//단순 스텟증가
//조건부 스텟증가
//HitEffect 타격 확률,8%확률로 아군 몬스터 소환
//유니크 이펙트 구조를 바꾸지말고 모든것이 가능하게 만들어야함
//옵션 전체 이펙트로 만들면 되지않음?

//큰효과 나오는것은 GrantAbility
//문제는 일반스텟증가가 아닌 케이스

//타격확륭은 결국 델리게이트 연결,어빌리티

//그냥 이펙트
//그냥 이펙트에 조건부
//이펙트 그랜트 어빌리티

USTRUCT(BlueprintType) //난이도,티어
struct FOption : public FTableRowBase
{
    GENERATED_BODY()

public:
    FOption(): m_OptionIcon(nullptr)
    {
        m_OptionID = "SetSameWithRowName";
        m_FormatArguSet = "{0}{1}{2}"; //need open?
        m_FormatEffect = FText::FromString("Ex)% Attack Bonus");//This is last format
        m_bIsPercentValue = false;
        m_fMinValue=0.7f;
        m_fMaxValue=1.1f;
    }
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName m_OptionID;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UTexture* m_OptionIcon;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText m_FormatEffect;
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
    FString m_FormatArguSet;
    //
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
    FGameplayTag m_TagAttribute;
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)//만일 이게 없으면?
    FScalableFloat m_fLevelPerValue;
    //민맥스 레인지
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float m_fMinValue;//35 -> 35% -> 1.35
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float m_fMaxValue;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool m_bIsPercentValue;
    //
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
    TSubclassOf<UPlayerDiabloAbility> m_GrantAbility;
    //
protected:
    static FText Plus;
    
    static FText Minus;

public:
    FText GetOptionFormat(float optionValue) const
    {
        FFormatOrderedArguments Args;

        if (optionValue == 0)
        {
            PRINTF("WTF - Value Is Zero");
            Args.Add(Plus);
        }
        else if (optionValue < 0)
        {
            Args.Add(Minus);
        }
        else
        {
            Args.Add(Plus);
        }

        if (m_bIsPercentValue)
        {
            optionValue -= 1.f;
            optionValue *= 100.f;
            //1.35 -> 0.35 -> 35%
        }

        Args.Add(UKismetTextLibrary::Conv_FloatToText(optionValue, ERoundingMode::FromZero,
                                                      false, true, 1, 324, 1, 1));

        Args.Add(m_FormatEffect);

        FTextFormat FormatT = FText::FromString(m_FormatArguSet);

        return FText::Format(FormatT, Args);
    }

    FOptionSpec MakeOptionInst(int level) const 
    {
        static const FString ContextString = TEXT("Option::FScalableFloat::IsValid11");

        FOptionSpec NewOption;

        if(!m_fLevelPerValue.Curve.IsNull())
        {
            NewOption.m_fValue = m_fLevelPerValue.GetValueAtLevel(level, &ContextString) * FMath::RandRange(m_fMinValue,m_fMaxValue);    
        }
        else
        {
            NewOption.m_fValue = FMath::RandRange(m_fMinValue,m_fMaxValue);//주로 10% ~ 20% 같은 옵션
        }
        
        NewOption.m_OptionID = m_OptionID;

        NewOption.m_DataOption = this;

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

    static TSubclassOf<UItemOptionGameEffect> GetGEItemOption;

public:
    static const FOption& GetOption(FName id);

    static const FOption* GetOptionPtr(FName id);

    static FText GetOptionText(const FOptionSpec& spec);
};


//옵션의 포맷 효과 텍스트
//옵션 값의 최소 최대치
//옵션의 실제 이펙트
