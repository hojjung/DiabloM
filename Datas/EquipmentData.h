#pragma once
#include "DiabloM.h"
#include "GachaDataTable.h"
#include "Engine/DataTable.h"
#include "Item/AccessoryOption.h"
#include "Item/EquipmentActor.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Managers/PlayerUpgradeManager.h"
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AEquipmentActor> m_ClassVisualActor = nullptr;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nDmgLevel = 1;//1당 5배 
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fBaseCost = 9;

public:

	BigInt GetDmgPer(int level) const
	{
		level = FMath::Clamp(level,0,100);
		
		BigInt Dmg = 100;

		if(m_nDmgLevel>1)
		{
			int IterMax = m_nDmgLevel-1;
			
			for(int i=0; i<IterMax;i++)
			{
				Dmg.MultiplyFast(50);	
			}
		}

		if(level>0)
		{
			
		BigInt Percent = Dmg;
		
		Percent.Divide(100);
		
		Percent.Multiply(level);

		Dmg+=Percent;
		}

		return Dmg;
	}

	BigInt GetCost(int level) const
	{
		level  = FMath::Clamp(level,0,100);
		
		BigInt Cost = m_fBaseCost*level;

		if(level>1)
		{
			int IterMax = level-1;
			
			for(int i=0; i<IterMax;i++)
			{
				//Cost.MultiplyFast(2);
				Cost = UDiaBlueprintFunctionLibrary::MultiplePercent(Cost,107,0,2);
			}
		}

		return Cost;
	}

	FText GetFormatDescPreview(int level) const
	{
		FTextFormat Format = FText::FromString(TEXT("데미지 증가:{0}%>\n\r>다음 레벨{1}%"));

		FFormatOrderedArguments Args;
		
		FText Str1 =FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(GetDmgPer(level),2));
		
		Args.Add(Str1);

		int NewLevel = level+1;

		FText Str3 = FText::FromString("MAX");
		
		if(level<100)
		{
			FText Str2 =FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(GetDmgPer(NewLevel),2));
			
			Args.Add(Str2);
		}
		else
		{			
			Args.Add(Str3);
			//Args.Add(Str3);
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
		FTextFormat Format = FText::FromString(TEXT("공격속도 {0}%"));

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
		FTextFormat Format = FText::FromString(TEXT("이동속도 {0}%"));

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
		FTextFormat Format = FText::FromString(TEXT("골드 획득 보너스 {0}%"));

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




