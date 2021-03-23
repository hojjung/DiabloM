#pragma once
#include "DiabloM.h"
#include "GachaDataTable.h"
#include "Engine/DataTable.h"
#include "Item/AccessoryOption.h"
#include "Item/EquipmentActor.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "UObject/NoExportTypes.h"
#include "EquipmentData.generated.h"


UCLASS()
class DIABLOM_API UEquipmentData : public UObject
{
	GENERATED_BODY()
};
//

USTRUCT(BlueprintType)
struct FWeaponTable : public FGachaAbleRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly ,meta = (MultiLine = true))
	FText m_UpgradeDescFormat =FText::FromString( "Dmg Increase:{0}%>>P{1}%\nAccuracy Increase:{2}>>P{3}");
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AEquipmentActor> m_ClassVisualActor = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "1.0"))
	float m_fBaseDmgPer = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "1.0"))
	int m_nInitAccuracy = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "1.0"))
	int m_nBaseAccu = 1;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nMaxLevel = 100;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fBaseCost = 9;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fCostMultiFactor = 1.06f;

	BigInt GetDmgPer(int level) const
	{
		BigInt Dmg = m_fBaseDmgPer;

		Dmg.Multiply(level);
		
		return Dmg;
	}

	int GetAccuracy(int level) const
	{
		return (m_nBaseAccu*level) + m_nBaseAccu;
	}

	BigInt GetCost(int level) const
	{
		level  = FMath::Clamp(level,level,m_nMaxLevel);

		float Factor =  FMath::Pow(m_fCostMultiFactor,level);

		BigInt Cost = m_fBaseCost;

		Cost.Multiply(Factor);

		return Cost;
	}

	FText GetFormatDescPreview(int level) const
	{
		FTextFormat Format = m_UpgradeDescFormat;

		FFormatOrderedArguments Args;
		
		FText Str1 =FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(GetDmgPer(level)));
		
		Args.Add(Str1);

		int NewLevel = level+1;

		FText Str3 = FText::FromString("MAX");
		
		if(level<m_nMaxLevel)
		{
			FText Str2 =FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(GetDmgPer(NewLevel)));
			
			Args.Add(Str2);
			
				
		}
		else
		{			
			Args.Add(Str3);
			Args.Add(GetAccuracy(level));
			Args.Add(Str3);
		}

		FText TT = FText::Format(Format,Args);

		return TT; 
	}
};


USTRUCT(BlueprintType)
struct FPlayerSkinTable : public FGachaAbleRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta = (MultiLine = true))
	FString m_DescFormat = "Attack Speed {0}%";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMesh* m_PlayerSkin = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> m_AnimBP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimSequence* m_VisualIdleAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* m_BaseAttackAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "1.0", UIMax = "3.3"))
	float m_fAttackSpeedMultiple = 1.f;

	float GetAtkSpdBonus() const
	{
		return m_fAttackSpeedMultiple;
	}

	FText GetFormatDescPreview() const
	{
		FTextFormat Format = FText::FromString(m_DescFormat);

		FFormatOrderedArguments Args;
		
		Args.Add((GetAtkSpdBonus()*100.f) - 100.f);

		FText TT = FText::Format(Format,Args);

		return TT; 
	}
};



USTRUCT(BlueprintType)
struct FWingTable : public FGachaAbleRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta = (MultiLine = true))
	FString m_DescFormat = "Move Speed Increase:{0}%";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AEquipmentActor> m_ClassVisualWingActor = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "1.0", UIMax = "3.3"))
	float m_fWingMoveSpeedMultiple = 1.f;

	float GetMoveSpdBonus() const
	{
		return m_fWingMoveSpeedMultiple;
	}

	FText GetFormatDescPreview() const
	{
		FTextFormat Format = FText::FromString(m_DescFormat);

		FFormatOrderedArguments Args;
		
		Args.Add((GetMoveSpdBonus()*100.f) - 100.f);

		FText TT = FText::Format(Format,Args);

		return TT; 
	}
};

USTRUCT(BlueprintType)
struct FPetTable : public FGachaAbleRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta = (MultiLine = true))
	FString m_DescFormat = "Gold Gain Increase:{0}%";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AEquipmentActor> m_ClassPetSkin;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nMaxLevel = 100;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fInitValue = 0;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)//인컴은 항상 선형적이다
	float m_fBaseValue = 1.67f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fBaseCost = 9;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fCostMultiFactor = 1.06f;
	
public:
	BigInt GetCost(int level) const
	{
		level  = FMath::Clamp(level,level,m_nMaxLevel);

		float Factor =  FMath::Pow(m_fCostMultiFactor,level);

		BigInt Cost = m_fBaseCost;

		Cost.Multiply(Factor);

		return Cost;
	}

	BigInt GetGoldBonusValue(int level) const
	{
		level  = FMath::Clamp(level,level,m_nMaxLevel);
		
		BigInt Value = m_fBaseValue;

		return (Value * level) + m_fInitValue;
	}

	FText GetFormatDescPreview(int level) const
	{
		FTextFormat Format = FText::FromString(m_DescFormat);

		FText Str1 =FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(GetGoldBonusValue(level)));
		
		FFormatOrderedArguments Args;
		
		Args.Add(Str1);

		int NewLevel = level+1;
		
		if(level<m_nMaxLevel)
		{
			FText Str2 =FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(GetGoldBonusValue(NewLevel)));
			
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

USTRUCT(BlueprintType)
struct FAccessoryTable : public FGachaAbleRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta = (MultiLine = true))
	FString m_UpgradeDescFormat = "Current:{0}>>P{1}";
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nMaxLevel = 100;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fInitValue = 0;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)//인컴은 항상 선형적이다
	float m_fBaseValue = 1.67f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fBaseCost = 9;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fCostMultiFactor = 1.06f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UAccessoryOption> m_ClassAccessoryOp;
	
public:
	virtual BigInt GetValue(int level) const
	{
		level  = FMath::Clamp(level,level,m_nMaxLevel);
		
		BigInt Value = m_fBaseValue;

		return (Value * level) + m_fInitValue;
	}

	int GetCost(int level) const
	{
		return level;
	}

	FText GetFormatDescPreview(int level) const
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




