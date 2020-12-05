#pragma once
#include "DiabloM.h"
#include "ItemDataTable.h"
#include "AbilitySystem/Ability/DiabloAbility.h"
#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"
#include "SkillDataTable.generated.h"


UCLASS()
class DIABLOM_API USkillDataTable : public UObject
{
	GENERATED_BODY()
public:
	USkillDataTable();
	
public:
	static  UDataTable* GetSkillDataTable;
};

USTRUCT(BlueprintType)
struct FLevelupableScaleFloat
{
	GENERATED_BODY()
public:
	FLevelupableScaleFloat()
	{
		m_bIsLevelupable=false;
		m_bUseRandRange=false;
		m_fRandRange=0.2f;
	}

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat m_fScaleFloat;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool m_bIsLevelupable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool m_bUseRandRange;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "0.0", UIMax = "1.0"))
	float m_fRandRange;

	FText GetRangeFormatText(int level) const
	{
		float Value = m_fScaleFloat.GetValueAtLevel(level);
		float MinValue = Value* (1.f-m_fRandRange);
		float MaxValue = Value* (1.f+m_fRandRange);
		FTextFormat RangeFormat = FText::FromString("{0}-{1}");

		FFormatOrderedArguments Args;

		Args.Add(MinValue);

		Args.Add(MaxValue);
		
		return FText::Format(RangeFormat, Args);
	}
};

USTRUCT(BlueprintType)
struct FSkillData
{
	GENERATED_BODY()
public:
	FSkillData(): m_SkillAbility(nullptr), m_SkillIcon(nullptr)
	{
		m_nMinRequireLevel=0;
		m_nMaxSkillLevel=15;
		m_FormatSkillDesc = FText::FromString("Ex)% Attack Bonus {0}");//This is last format
		m_FormatSkillCost = FText::FromString("Ex)Generate Fury:{0}");//This is last format
		m_FormatSkillCD = FText::FromString("Cooldown:{0}");//This is last format
		m_FormatSkillRank= FText::FromString("Rank:{0}");//This is last format
	}

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDiabloAbility> m_SkillAbility;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* m_SkillIcon;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_TagSkillCastType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_TagSkillDamageType;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_SkillShowingName;
	FText m_FormatSkillCD;
	FText m_FormatSkillRank;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_FormatSkillCost;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=( MultiLine="true" ))
	FText m_FormatSkillDesc;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=( MultiLine="true" ))
	FText m_FormatSkillPreviewLevelup;
	//요구 스탠스 및 주무기 보조무기 종류
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nMinRequireLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nMaxSkillLevel;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)//cost  표시 어떻게?
	FLevelupableScaleFloat m_fSkillCost;//블프랑 코스트호환?//음수?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)//cost  표시 어떻게?
	FLevelupableScaleFloat m_fSkillCD;//블프랑 코스트호환?//음수?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)//cost  표시 어떻게?
	TArray<FLevelupableScaleFloat> m_ArySkillValue;//블프랑 코스트호환?//음수?
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemTypeHandle> m_RequireMainWeaponType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemTypeHandle> m_RequireSubWeaponType;
	//플레이어 어트리뷰트에 의해 줄어든 수치 어떻게?
	//데이터 테이블이아니라 옵션 스펙이 포맷을 만드는게 맞을듯 하다.
	//모든 수치가 아이템 옵션 패시브 스킬, 특성으로 변경될수있다.
	
};

//시작하면 모든 데이터 테이블에 맞춰서 스펙을 만들어야함,저장되는것은 인덱스와 레벨뿐이다
USTRUCT(BlueprintType)
struct FSkillDataSpec
{
	GENERATED_BODY()
public:
	FSkillDataSpec()
	{
		m_SkillDataPtr = nullptr;
		m_nCurrentLevel = 0;
		m_fCurrentCDRemain=0.f;
	}
	
	FSkillDataSpec(int level,const FSkillData* skillData)
	{
		m_SkillDataPtr = skillData;
		m_nCurrentLevel = level;
		m_fCurrentCDRemain=0.f;
	}

public:
	UPROPERTY(EditAnywhere)
	int m_nCurrentLevel;
	
	const FSkillData* m_SkillDataPtr;
	
	float m_fCurrentCDRemain;

public:
	FText GetCostText() const
	{
		float Cost = m_SkillDataPtr->m_fSkillCost.m_fScaleFloat.GetValueAtLevel(m_nCurrentLevel);
		
		FFormatOrderedArguments Args;

		float AbsCost= FMath::Abs(Cost);

		Args.Add(AbsCost);
		
		FTextFormat FormatT = m_SkillDataPtr->m_FormatSkillCost;
 
		return FText::Format(FormatT, Args);
	}

	bool GetCoolDownText(FText& outText) const
	{
		float CD = m_SkillDataPtr->m_fSkillCD.m_fScaleFloat.GetValueAtLevel(m_nCurrentLevel);
		float CDRate =1.f - UPlayerDiabloAttribute::Get->GetCoolDownReduce();
		CD *=CDRate;
		
		if(CD<=0.f)
		{
			return false;
		}
        
		FFormatOrderedArguments Args;

		Args.Add(CD);
		
		FTextFormat FormatT = m_SkillDataPtr->m_FormatSkillCD;

		outText = FText::Format(FormatT, Args);

		return true;
	}
	//
	FText GetDescFormatText() const
	{
		FFormatOrderedArguments Args;
		
		for(auto& SFloat : m_SkillDataPtr->m_ArySkillValue)
		{
			if(SFloat.m_bUseRandRange)
			{
				Args.Add( SFloat.GetRangeFormatText(m_nCurrentLevel));
			}
			else
			{
				Args.Add( SFloat.m_fScaleFloat.GetValueAtLevel(m_nCurrentLevel));
			}
		}
		
		FTextFormat FormatT = m_SkillDataPtr->m_FormatSkillDesc;
 
		return FText::Format(FormatT, Args);
	}

	FText GetSkillRankText() const
	{
		FFormatOrderedArguments Args;

		Args.Add(m_nCurrentLevel);
		
		FTextFormat FormatT = m_SkillDataPtr->m_FormatSkillRank;
 
		return FText::Format(FormatT, Args);
	}

	FText GetLevelupPreviewFormatText() const
	{
		int NextSkillLevel = m_nCurrentLevel+1;
		
		FFormatOrderedArguments Args;

		if(m_SkillDataPtr->m_fSkillCost.m_bIsLevelupable)
		{
			Args.Add(m_SkillDataPtr->m_fSkillCost.m_fScaleFloat.GetValueAtLevel(NextSkillLevel));
		}

		if(m_SkillDataPtr->m_fSkillCD.m_bIsLevelupable)
		{
			Args.Add(m_SkillDataPtr->m_fSkillCD.m_fScaleFloat.GetValueAtLevel(NextSkillLevel));
		}

		for(auto& SFloat : m_SkillDataPtr->m_ArySkillValue)
		{
			if(!SFloat.m_bIsLevelupable)
			{
				continue;
			}

			if(SFloat.m_bUseRandRange)
			{
				Args.Add( SFloat.GetRangeFormatText(NextSkillLevel));
			}
			else
			{
				Args.Add( SFloat.m_fScaleFloat.GetValueAtLevel(NextSkillLevel));
			}
		}
 
		FTextFormat FormatT = m_SkillDataPtr->m_FormatSkillPreviewLevelup;
 
		return FText::Format(FormatT, Args);
	}
};

USTRUCT(BlueprintType)
struct FSkillDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_BaseSkillBeltName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FSkillData> m_AryBaseSkillBelt;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_PowerSkillBeltName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FSkillData> m_AryPowerSkillBelt;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_DefensiveSkillBeltName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FSkillData> m_AryDefensiveSkillBelt;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_SpecialSkillBeltName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FSkillData> m_ArySpecialSkillBelt;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_MasterySkillBeltName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FSkillData> m_AryMasterySkillBelt;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_UltimateSkillBeltName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FSkillData> m_AryUltimateSkillBelt;
};

USTRUCT(BlueprintType)
struct FSkillDataHandle :public FDataTableRowHandle
{
	GENERATED_USTRUCT_BODY()
public:
	FSkillDataHandle()
	{
		DataTable=USkillDataTable::GetSkillDataTable;
	}

};
