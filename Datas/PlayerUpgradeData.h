#pragma once

#include "DiabloM.h"
#include "SCUE4.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "UObject/NoExportTypes.h"
#include "PlayerUpgradeData.generated.h"

class ISkillUseCharacter;
UCLASS()
class DIABLOM_API UPlayerUpgradeData : public UObject
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FUpgradeDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UTexture2D* m_UpgradeIcon;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText m_UpgradeShowName;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FString m_UpgradeDescFormat = "Current:{0}>>P{1}";
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nMaxLevel = 400;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool m_bUseLevelBonus = true;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool m_bUseExponent = true;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fInitValue = 30;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)//인컴은 항상 선형적이다
	float m_fValueMultipleBase = 30;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nValueMultipleFactor00 = 106;//this value is float,like 167 = 1.67
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fBaseCost = 9;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nCostMultiFactor00 = 106;//this value is float,like 167 = 1.67


public:
	virtual BigInt GetValue(int level) const//소수점 표기 어떻게
	{
		level  = FMath::Clamp(level,level,m_nMaxLevel);
		
		BigInt Value = (m_fValueMultipleBase*level)+m_fInitValue;

		if(m_bUseExponent&&level>1)
		{
			int IterMax = level-1;
			
			for(int i=0; i<IterMax;i++)
			{
				Value = UDiaBlueprintFunctionLibrary::MultiplePercent(Value,m_nValueMultipleFactor00,0,2);
			}
		}

		return Value;
	}

	BigInt GetCost(int level) const
	{
		BigInt Cost = m_fBaseCost;
		
		level  = FMath::Clamp(level,level,m_nMaxLevel);

		if(level>1)
		{
			int IterMax = level-1;
			
			for(int i=0; i<IterMax;i++)
			{
				Cost = UDiaBlueprintFunctionLibrary::MultiplePercent(Cost,m_nCostMultiFactor00,0,2);
			}
		}

		return Cost;
	}

	virtual FText GetFormatDescPreview(int level) const
	{
		FTextFormat Format = FText::FromString(m_UpgradeDescFormat);

		FText Str1 =FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(GetValue(level),2));
		
		FFormatOrderedArguments Args;
		
		Args.Add(Str1);

		int NewLevel = level+1;
		
		if(level<m_nMaxLevel)
		{
			FText Str2 =FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(GetValue(NewLevel),2));
			
			Args.Add(Str2);
		}
		else
		{
			FText Str3 = FText::FromString("MAX");
			
			Args.Add(Str3);
		}

		FText TT = FText::Format(Format,Args);

		return TT; 
	}
};

USTRUCT()
struct FUpgradeSpec
{
	GENERATED_BODY()
public:
	FSafeInt m_nLv;
	const FUpgradeDataRow* m_UpgradeData;
	BigInt m_Value;
	BigInt m_Cost;

	bool IsUpgradeAble()
	{
		return  m_nLv < GetMaxLv();
	}

	void IncreaseLevel()
	{
		++m_nLv;
		SetLevel(m_nLv.GetValue());
	}

	void SetLevel(int v)
	{
		m_nLv = v;
		m_Value = m_UpgradeData->GetValue(m_nLv.GetValue());
		m_Cost = m_UpgradeData->GetCost(m_nLv.GetValue());
	}

	int GetMaxLv() const
	{
		return m_UpgradeData->m_nMaxLevel;
	}
};

USTRUCT(BlueprintType)
struct FSkillUpgradeDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName m_IDSkillMontageSection;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nSectionIndex;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fSkillCoolTime = 10.f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UTexture2D* m_SkillIcon;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText m_SkillShowName;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly ,meta = (MultiLine="true"))
	FString m_SkillDescFormat = "Current:{0}>>P{1}";
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool m_bUseExponent = true;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nMaxLevel = 400;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool m_bUseLevelBonus = true;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fInitValue = 0;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)//인컴은 항상 선형적이다
	float m_fValueMultipleBase = 30;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nValueMultipleFactor00 = 106;//this value is float,like 167 = 1.67
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fBaseCost = 9;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nCostMultiFactor00 = 106;//this value is float,like 167 = 1.67
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fRageCost = 25;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool m_bIsRootmotion = false;

public:
	virtual BigInt GetValue(int level) const
	{
		level  = FMath::Clamp(level,level,m_nMaxLevel);
		
		BigInt Value = (m_fValueMultipleBase*level)+m_fInitValue;

		if(m_bUseExponent&&level>1)
		{
			int IterMax = level-1;
			
			for(int i=0; i<IterMax;i++)
			{
				Value = UDiaBlueprintFunctionLibrary::MultiplePercent(Value,m_nValueMultipleFactor00,0,2);
			}
		}

		return Value;
	}

	int GetCost(int level) const
	{
		int Cost = m_fBaseCost;
		
		level  = FMath::Clamp(level,level,m_nMaxLevel);

		if(level>1)
		{
			int IterMax = level-1;

			Cost*=IterMax;
		}

		return Cost;
	}

	virtual FText GetFormatDescPreview(int level) const
	{
		FTextFormat Format = FText::FromString(m_SkillDescFormat);

		FText Str1 =FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(GetValue(level)));
		
		FFormatOrderedArguments Args;
		
		Args.Add(Str1);

		int NewLevel = level+1;
		
		if(level<m_nMaxLevel)
		{
			FText Str2 =FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(GetValue(NewLevel)));
			
			Args.Add(Str2);
		}
		else
		{
			FText Str3 = FText::FromString("MAX");
			
			Args.Add(Str3);
		}

		FText TT = FText::Format(Format,Args);

		return TT; 
	}
};

USTRUCT()
struct FSkillSpec
{
	GENERATED_BODY()
public:
	int m_nIndex = -1;
	FSafeInt m_nLv;
	const FSkillUpgradeDataRow* m_SkillData;
	BigInt m_Value;
	int m_Cost;
	float m_fCurrentCD;
	float m_fDuration;

	bool IsSkillUseable(ISkillUseCharacter* skillOwner);
	

	void IncreaseLevel()
	{
		++m_nLv;
		SetLevel(m_nLv.GetValue());
	}

	void SetLevel(int v)
	{
		m_nLv = v;
		m_Value = m_SkillData->GetValue(m_nLv.GetValue());
		m_Cost = m_SkillData->GetCost(m_nLv.GetValue());
	}

	int GetMaxLv() const
	{
		return m_SkillData->m_nMaxLevel;
	}

	float UseSkill(ISkillUseCharacter* skillOwner);


	void Tick(float delta_time)
	{
		m_fCurrentCD-=delta_time;
		
	}

	bool IsUpgradeAble()
	{
		return m_nLv < GetMaxLv();
	}

	FString ParseToString()
	{
		FString StrW;
		StrW.AppendInt(m_nLv.GetValue());
		StrW.Append(":");
		StrW.AppendInt(m_nIndex);
		StrW.Append(":");

		return StrW;
	}

	void InitSkillSpec(int lv, int index)
	{
		SetLevel(lv);
		m_nIndex = index;
	}

	bool IsCooldownReady()
	{
		return m_fCurrentCD<=0;
	}
};