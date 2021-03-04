#pragma once

#include "DiabloM.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "UObject/NoExportTypes.h"
#include "PlayerUpgradeData.generated.h"

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
	float m_fInitValue = 0;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)//인컴은 항상 선형적이다
	float m_fBaseValue = 1.67f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fBaseCost = 9;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fCostMultiFactor = 1.06f;

private:
	float GetLevelBonus(int level) const
	{
		if(!m_bUseLevelBonus)
		{
			return 1.f;
		}
		if(level < 25)
		{
			return 2.f;
		}
		else if(level < 50)
		{
			return 4.f;
		}
		else if(level < 100)
		{
			return 8.f;
		}
		else if(level < 200)
		{
			return 16.f;
		}
		else if(level < 300)
		{
			return 32.f;
		}
		else if(level < 400)
		{
			return 64.f;
		}
		else if(level < 500)
		{
			return 128.f;
		}
		else if(level < 600)
		{
			return 256.f;
		}
		else if(level < 1000)
		{
			return 512.f;
		}
		else 
		{
			return 1024.f;
		}
	}

public:
	virtual BigInt GetValue(int level) const
	{
		level  = FMath::Clamp(level,level,m_nMaxLevel);
		
		float BonusMulti = GetLevelBonus(level);

		BigInt Value = m_fBaseValue;

		return (Value * BonusMulti * level) + m_fInitValue;
	}

	BigInt GetCost(int level) const
	{
		level  = FMath::Clamp(level,level,m_nMaxLevel);

		float Factor =  FMath::Pow(m_fCostMultiFactor,level);

		BigInt Cost = m_fBaseCost;

		Cost.Multiply(Factor);

		return Cost;
	}

	virtual FText GetFormatDescPreview(int level) const
	{
		FTextFormat Format = FText::FromString(m_UpgradeDescFormat);

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
struct FUpgradeSpec
{
	GENERATED_BODY()
public:
	int m_nLv;
	const FUpgradeDataRow* m_UpgradeData;
	BigInt m_Value;
	BigInt m_Cost;

	void IncreaseLevel()
	{
		m_nLv++;
		SetLevel(m_nLv);
	}

	void SetLevel(int v)
	{
		m_nLv = v;
		m_Value = m_UpgradeData->GetValue(m_nLv);
		m_Cost = m_UpgradeData->GetCost(m_nLv);
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
	UTexture2D* m_SkillIcon;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText m_SkillShowName;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FString m_SkillDescFormat = "Current:{0}>>P{1}";
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool m_bIsDragSkill = false;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nMaxLevel = 400;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool m_bUseLevelBonus = true;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fInitValue = 0;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)//인컴은 항상 선형적이다
	float m_fBaseValue = 1.67f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fBaseCost = 9;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fCostMultiFactor = 1.06f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fRageCost = 25;

private:
	float GetLevelBonus(int level) const
	{
		if(!m_bUseLevelBonus)
		{
			return 1.f;
		}
		if(level < 25)
		{
			return 2.f;
		}
		else if(level < 50)
		{
			return 4.f;
		}
		else if(level < 100)
		{
			return 8.f;
		}
		else if(level < 200)
		{
			return 16.f;
		}
		else if(level < 300)
		{
			return 32.f;
		}
		else if(level < 400)
		{
			return 64.f;
		}
		else if(level < 500)
		{
			return 128.f;
		}
		else if(level < 600)
		{
			return 256.f;
		}
		else if(level < 1000)
		{
			return 512.f;
		}
		else 
		{
			return 1024.f;
		}
	}

public:
	virtual BigInt GetValue(int level) const
	{
		level  = FMath::Clamp(level,level,m_nMaxLevel);
		
		float BonusMulti = GetLevelBonus(level);

		BigInt Value = m_fBaseValue;

		return (Value * BonusMulti * level) + m_fInitValue;
	}

	BigInt GetCost(int level) const
	{
		level  = FMath::Clamp(level,level,m_nMaxLevel);

		float Factor =  FMath::Pow(m_fCostMultiFactor,level);

		BigInt Cost = m_fBaseCost;

		Cost.Multiply(Factor);

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
	int m_nLv;
	const FSkillUpgradeDataRow* m_SkillData;
	BigInt m_Value;
	BigInt m_Cost;

	void IncreaseLevel()
	{
		m_nLv++;
		SetLevel(m_nLv);
	}

	void SetLevel(int v)
	{
		m_nLv = v;
		m_Value = m_SkillData->GetValue(m_nLv);
		m_Cost = m_SkillData->GetCost(m_nLv);
	}

	int GetMaxLv() const
	{
		return m_SkillData->m_nMaxLevel;
	}
};