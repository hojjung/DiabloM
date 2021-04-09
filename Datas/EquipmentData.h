#pragma once
#include "DiabloM.h"
#include "GachaDataTable.h"
#include "Engine/DataTable.h"
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
		
		BigInt Dmg = 220;

		if(m_nDmgLevel>1)
		{
			int IterMax = m_nDmgLevel-1;
			
			for(int i=0; i<IterMax;i++)
			{
				Dmg.MultiplyFast(34);	
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

		if(m_nDmgLevel>1)
		{
			int IterMax = m_nDmgLevel-1;
			
			for(int i=0; i<IterMax;i++)
			{
				//Cost.MultiplyFast(2);
				Cost = UDiaBlueprintFunctionLibrary::MultiplePercent(Cost,150,0,2);
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
	TSubclassOf<UAnimInstance> m_AnimBP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimSequence* m_VisualIdleAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* m_BaseAttackAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMesh> m_PlayerSkinSoft;
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
	int m_nFactorLevel=1;
	
public:
	BigInt GetCost(int level) const
	{
		level  = FMath::Clamp(level,0,100);
		
		BigInt Cost = 900*level;

		if(m_nFactorLevel>1)
		{
			int IterMax = m_nFactorLevel-1;
			
			for(int i=0; i<IterMax;i++)
			{
				//Cost.MultiplyFast(8);
				Cost = UDiaBlueprintFunctionLibrary::MultiplePercent(Cost,150,0,2);
			}
		}

		return Cost;
	}

	BigInt GetGoldBonusValue(int level) const
	{
		level = FMath::Clamp(level,0,100);
		
		BigInt GoldBonus = 120;

		if(m_nFactorLevel>1)
		{
			int IterMax = m_nFactorLevel-1;
			
			for(int i=0; i<IterMax;i++)
			{
				GoldBonus = UDiaBlueprintFunctionLibrary::MultiplePercent(GoldBonus,175,0,2);
			}
		}

		if(level>0)//100렙찍을때 두배증가
		{
			BigInt Percent = GoldBonus;
		
			Percent.Divide(100);
		
			Percent.Multiply(level);

			GoldBonus+=Percent;
		}

		return GoldBonus;
	}

	FText GetFormatDescPreview(int level) const
	{
		FTextFormat Format = FText::FromString(TEXT("골드보너스:{0}%"));

		FText Str1 =FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(GetGoldBonusValue(level),2));
		
		FFormatOrderedArguments Args;
		
		Args.Add(Str1);

		int NewLevel = level+1;
		
		if(level<100)
		{
			FText Str2 =FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(GetGoldBonusValue(NewLevel),2));
			
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
	int m_nInitValue = 0;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)//인컴은 항상 선형적이다
	int m_nBaseValue = 100;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nID;
	
public:
	virtual BigInt GetValue(int level) const
	{
		level  = FMath::Clamp(level,level,500);
		
		BigInt Value = m_nBaseValue;

		return (Value * level) + m_nInitValue;
	}

	float GetFloatValue(int level)const
	{
		level  = FMath::Clamp(level,level,500);
		
		float Value = m_nBaseValue;

		return ((Value * level) + m_nInitValue) / 100.f;
	}

	int GetCost(int level) const
	{
		return level;
	}

	FText GetFormatDescPreview(int level) const
	{
		FTextFormat Format = FText::FromString(m_UpgradeDescFormat);

		FText Str1 = FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(GetValue(level),2));
		
		FFormatOrderedArguments Args;
		
		Args.Add(Str1);

		int NewLevel = level+1;
		
		if(level<500)
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




